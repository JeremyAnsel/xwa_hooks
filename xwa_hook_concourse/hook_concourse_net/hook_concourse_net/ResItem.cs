using System.Collections.Generic;

namespace hook_concourse_net
{
    public sealed class ResItem
    {
        public ResItemDescription DescriptionSD { get; set; }

        public List<ResItemImage> ImagesHD { get; } = new List<ResItemImage>();
    }
}
