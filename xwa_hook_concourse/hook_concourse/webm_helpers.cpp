#include "targetver.h"
#include "webm_helpers.h"
#include <iostream>
#include <cstdarg>
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <Windows.h>

extern "C" {
#include "libvpx\vpx\vpx_decoder.h"
#include "libvpx\vpx\vp8dx.h"
#include "libvpx\vpx\vpx_image.h"
#include "libyuv/convert_argb.h"
#include "nestegg/nestegg.h"

	void usage_exit()
	{
		exit(-1);
	}
}

#pragma comment(lib, "libvpx")
#pragma comment(lib, "libyuv")
#pragma comment(lib, "nestegg")

static unsigned int mem_get_le32(const unsigned char* mem) {
	return (mem[3] << 24) | (mem[2] << 16) | (mem[1] << 8) | (mem[0]);
}

void logger(nestegg* ctx, unsigned int severity, char const* fmt, ...) {
	va_list ap;
	char const* sev = NULL;

	switch (severity) {
	case NESTEGG_LOG_DEBUG:
		sev = "debug:   ";
		break;
	case NESTEGG_LOG_WARNING:
		sev = "warning: ";
		break;
	case NESTEGG_LOG_CRITICAL:
		sev = "critical:";
		break;
	default:
		sev = "unknown: ";
	}

	fprintf(stderr, "%p %s ", (void*)ctx, sev);

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fprintf(stderr, "\n");

	char msg[1024];
	va_start(ap, fmt);
	vsprintf_s(msg, fmt, ap);
	va_end(ap);
	OutputDebugString(msg);
}

class DataFile
{
public:
	DataFile();
	void Open(const std::string& filename);
	void Close();
	bool IsEof();
	int Read(void* buffer, size_t size);
	int Seek(size_t n, int whence);
	size_t Tell();
	bool IsValid();

private:
	bool _isValid;
	size_t _position;
	std::vector<char> _data;
};

DataFile::DataFile()
{
	_isValid = false;
	_position = 0;
}

void DataFile::Open(const std::string& filename)
{
	Close();

	std::ifstream infile;
	infile.open(filename, std::ios::binary);

	if (!infile.is_open())
	{
		return;
	}

	infile.seekg(0, std::ios_base::_Seekend);
	size_t size = (size_t)infile.tellg();
	infile.seekg(0, std::ios_base::_Seekbeg);

	_data.reserve(size);
	infile.read(_data.data(), size);

	infile.close();
	_isValid = true;
}

void DataFile::Close()
{
	if (!_isValid)
	{
		return;
	}

	_isValid = false;
	_position = 0;
	//_data.reserve(0);
}

bool DataFile::IsEof()
{
	return _isValid && (_position == _data.capacity());
}

int DataFile::Read(void* buffer, size_t size)
{
	// success = 1
	// eof = 0
	// error = -1

	if (!_isValid)
	{
		return -1;
	}

	size_t left = _data.capacity() - _position;

	if (left >= size)
	{
		memcpy(buffer, _data.data() + _position, size);
		_position += size;
		return 1;
	}

	memcpy(buffer, _data.data() + _position, left);
	_position += left;

	return 0;
}

int DataFile::Seek(size_t n, int whence)
{
	if (!_isValid)
	{
		return -1;
	}

	switch (whence)
	{
	case NESTEGG_SEEK_SET:
		_position = n;
		break;
	case NESTEGG_SEEK_CUR:
		_position += n;
		break;
	case NESTEGG_SEEK_END:
		_position = _data.capacity() - n;
		break;
	}

	if (_position > _data.capacity())
	{
		return -1;
	}

	return 0;
}

size_t DataFile::Tell()
{
	if (!_isValid)
	{
		return 0;
	}

	return _position;
}

bool DataFile::IsValid()
{
	return _isValid;
}

int datafile_read(void* buffer, size_t size, void* context)
{
	// success = 1
	// eof = 0
	// error = -1
	DataFile* f = (DataFile*)context;
	return f->Read(buffer, size);
}

int datafile_seek(int64_t n, int whence, void* context)
{
	DataFile* f = (DataFile*)context;
	return f->Seek((size_t)n, whence);
}

int64_t datafile_tell(void* context)
{
	DataFile* f = (DataFile*)context;
	return f->Tell();
}

class WebmVideoCache
{
public:
	~WebmVideoCache();

	bool IsOpened();
	void Open(const std::string& filename);
	void Close();
	void ResetTimecode();
	int ReadNextFrame();

	float GetDurationInSeconds()
	{
		return duration * 1e-9f;
	}

	float GetTimecodeInSeconds()
	{
		return timecode * 1e-9f;
	}

	bool isLoaded = false;
	std::string infilename;
	DataFile infile;
	nestegg* ne = 0;
	uint64_t duration = 0;
	uint64_t timecode = 0;
	vpx_codec_ctx_t codec_yuv{};
	vpx_codec_ctx_t codec_alpha{};
	unsigned int width = 0;
	unsigned int height = 0;
	std::vector<uint8_t> image;
};

WebmVideoCache::~WebmVideoCache()
{
	Close();
}

bool WebmVideoCache::IsOpened()
{
	return isLoaded;
}

void WebmVideoCache::Open(const std::string& filename)
{
	Close();

	infilename = filename;

	int r = 0;
	infile.Open(filename);

	if (!infile.IsValid())
	{
		return;
	}

	nestegg_io ne_io{};
	ne_io.read = datafile_read;
	ne_io.seek = datafile_seek;
	ne_io.tell = datafile_tell;
	ne_io.userdata = (void*)&infile;

	r = nestegg_init(&ne, ne_io, /*logger*/ 0, -1);
	assert(r == 0);

	r = nestegg_duration(ne, &duration);
	assert(r == 0);
	timecode = 0;

	int id = nestegg_track_codec_id(ne, 0);
	assert(id >= 0);
	int type = nestegg_track_type(ne, 0);
	assert(type == NESTEGG_TRACK_VIDEO);

	vpx_codec_iface_t* iface = id == NESTEGG_CODEC_VP9 ? &vpx_codec_vp9_dx_algo : &vpx_codec_vp8_dx_algo;

	nestegg_video_params vparams{};
	r = nestegg_track_video_params(ne, 0, &vparams);
	assert(r == 0);

	width = vparams.width;
	height = vparams.height;
	image.reserve(width * height * 4);

	r = vpx_codec_dec_init(&codec_yuv, iface, NULL, 0);
	assert(r == 0);

	r = vpx_codec_dec_init(&codec_alpha, iface, NULL, 0);
	assert(r == 0);

	isLoaded = true;
}

void WebmVideoCache::Close()
{
	if (!IsOpened())
	{
		return;
	}

	//image.reserve(0);

	vpx_codec_destroy(&codec_alpha);
	vpx_codec_destroy(&codec_yuv);

	codec_yuv = {};
	codec_alpha = {};

	if (ne)
	{
		nestegg_destroy(ne);
		ne = 0;
	}

	//infile->Close();
	isLoaded = false;
}

void WebmVideoCache::ResetTimecode()
{
	timecode = -1;

	if (!IsOpened() || !infile.IsValid())
	{
		return;
	}

	unsigned int tracks = 0;
	nestegg_track_count(ne, &tracks);

	for (unsigned int i = 0; i < tracks; i++)
	{
		nestegg_track_seek(ne, i, 0);
	}
}

int WebmVideoCache::ReadNextFrame()
{
	if (!IsOpened())
	{
		return 0;
	}

	int r = 0;
	nestegg_packet* packet = 0;

	while (true)
	{
		// 1 = keep calling
		// 0 = eof
		// -1 = error
		r = nestegg_read_packet(ne, &packet);

		if (r == 0)
		{
			ResetTimecode();
		}

		if (r <= 0)
			return r;

		unsigned int track = 0;
		r = nestegg_packet_track(packet, &track);
		assert(r == 0);

		int type = nestegg_track_type(ne, track);
		if (type != NESTEGG_TRACK_VIDEO)
			continue;

		if (r != 1 || packet != 0)
			break;
	}

	unsigned int track = 0;
	r = nestegg_packet_track(packet, &track);
	assert(r == 0);

	int type = nestegg_track_type(ne, track);
	assert(type == NESTEGG_TRACK_VIDEO);

	nestegg_packet_tstamp(packet, &timecode);

	unsigned char* data_yuv;
	size_t length_yuv;
	r = nestegg_packet_data(packet, 0, &data_yuv, &length_yuv);
	assert(r == 0);

	unsigned char* data_alpha;
	size_t length_alpha;
	r = nestegg_packet_additional_data(packet, 1, &data_alpha, &length_alpha);

	vpx_codec_err_t e_yuv = vpx_codec_decode(&codec_yuv, data_yuv, length_yuv, NULL, 0);
	vpx_codec_err_t e_alpha = vpx_codec_decode(&codec_alpha, data_alpha, length_alpha, NULL, 0);

	if (e_yuv || e_alpha)
	{
		nestegg_free_packet(packet);
		return -1;
	}

	vpx_codec_iter_t iter_yuv = NULL;
	vpx_image_t* img_yuv = vpx_codec_get_frame(&codec_yuv, &iter_yuv);

	vpx_codec_iter_t iter_alpha = NULL;
	vpx_image_t* img_alpha = vpx_codec_get_frame(&codec_alpha, &iter_alpha);

	if (img_alpha)
	{
		libyuv::I420AlphaToARGB(
			img_yuv->planes[0],
			img_yuv->stride[0],
			img_yuv->planes[1],
			img_yuv->stride[1],
			img_yuv->planes[2],
			img_yuv->stride[2],
			img_alpha->planes[0],
			img_alpha->stride[0],
			image.data(),
			width * 4,
			width,
			height,
			//1);
			0);
	}
	else
	{
		libyuv::I420ToARGB(
			img_yuv->planes[0],
			img_yuv->stride[0],
			img_yuv->planes[1],
			img_yuv->stride[1],
			img_yuv->planes[2],
			img_yuv->stride[2],
			image.data(),
			width * 4,
			width,
			height);
	}

	nestegg_free_packet(packet);

	return 1;
}

std::map<std::string, WebmVideoCache*> _webm_videos;

void WebmLoadVideo(const std::string& name)
{
	auto it = _webm_videos.find(name);

	if (it != _webm_videos.end())
	{
		return;
	}

	std::string path = "Resdata\\" + name + ".webm";

	WebmVideoCache* video = new WebmVideoCache();
	video->Open(path);
	_webm_videos.insert(std::make_pair(name, video));
}

void WebmFreeVideo(const std::string& name)
{
	auto it = _webm_videos.find(name);

	if (it == _webm_videos.end())
	{
		return;
	}

	if (it->second)
	{
		delete it->second;
	}

	_webm_videos.erase(it);
}

void WebmFreeAllVideos()
{
	//for (const auto& item : _webm_videos)
	//{
	//	if (item.second)
	//	{
	//		delete item.second;
	//	}
	//}

	//_webm_videos.clear();
}

void WebmResetAllTimecode()
{
	for (auto& item : _webm_videos)
	{
		//if (item.second)
		//{
		//	item.second->ResetTimecode();
		//}

		item.second->ResetTimecode();
	}
}

int64_t WebmGetTimecode(const std::string& name)
{
	auto it = _webm_videos.find(name);

	if (it == _webm_videos.end())
	{
		return -1;
	}

	if (!it->second->IsOpened() || !it->second->infile.IsValid())
	{
		return -1;
	}

	return it->second->timecode;
}

int WebmReadVideoFrame(const std::string& name, uint8_t** image, unsigned int* width, unsigned int* height)
{
	*image = 0;
	*width = 0;
	*height = 0;

	auto it = _webm_videos.find(name);

	if (it == _webm_videos.end())
	{
		return 0;
	}

	if (!it->second->IsOpened() || !it->second->infile.IsValid())
	{
		return 0;
	}

	int r = it->second->ReadNextFrame();

	*image = it->second->image.data();
	*width = it->second->width;
	*height = it->second->height;

	return r;
}
