#include "targetver.h"
#include "translate.h"

const char* Translate(const std::string& language, MsgId id)
{
	if (language == "it")
	{
		switch (id)
		{
		case ID_MSG_SFOILS_CLOSING_1: return "\03S-Foil in chiusura";
		case ID_MSG_SFOILS_OPENING_1: return "\03S-Foil in apertura";
		case ID_MSG_SFOILS_CLOSED_1: return "03Gli S-Foil hanno raggiunto la posizione [chiusa]";
		case ID_MSG_SFOILS_OPEN_1: return "\03Gli S-Foil hanno raggiunto la posizione [aperta]";
		case ID_MSG_SFOILS_NO_FIRE: return "\03I cannoni non possono sparare con gli S-Foil o i carrelli chiusi";
		case ID_MSG_SFOILS_CLOSING_2: return "\03Carrelli in apertura";
		case ID_MSG_SFOILS_OPENING_2: return "\03Carrelli in chiusura";
		case ID_MSG_SFOILS_CLOSED_2: return "\03I carrelli hanno raggiunto la posizione [aperta]";
		case ID_MSG_SFOILS_OPEN_2: return "\03I carrelli hanno raggiunto la posizione [chiusa]";
		case ID_MSG_NOT_EQUIPPED_SFOIL_1: return "\03La tua nave non è dotata di [S-Foil]";
		case ID_MSG_NOT_EQUIPPED_SFOIL_2: return "\03[Landing Gears] Sono già in chiusura o apertura";
		case ID_MSG_NOT_EQUIPPED_SFOIL_3: return "\03Chiudi [Landing Gear] prima di attivare gli [S-Foils]";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_1: return "\03La tua nave non è dotata di [Landing Gears]";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_2: return "\03[S-Foils] Sono già in chiusura o apertura";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_3: return "\03Chiudi [S-Foils] prima di attivare gli [Landing Gears]";
		case ID_MSG_HANGAR_TRACTOR_1: return "\04Chiudere S-Foils ed aprire i carrelli prima di entrare nell' hangar";
		case ID_MSG_HANGAR_TRACTOR_2: return "\04Chiudere gli S-Foils prima di entrare nell' hangar";
		case ID_MSG_HANGAR_TRACTOR_3: return "\04Aprire i carrelli prima di entrare nell' hangar";
		case ID_MSG_HANGAR_TRACTOR_4: return "\04Premi [Space] per attivate il raggio traente ed entrare nell' hangar";
		case ID_MSG_ASK_ABOUT_HYPER_1: return "\03Chiudere i carrelli prima di entrare nell' iperspazio";
		case ID_MSG_ASK_ABOUT_HYPER_2: return "\03Premi [SPACE] per l' hyperspazio su [*]";
		}
	}
	else if (language == "es")
	{
		switch (id)
		{
		case ID_MSG_SFOILS_CLOSING_1: return "\03S-Foils cierre";
		case ID_MSG_SFOILS_OPENING_1: return "\03S-Foils abriendo";
		case ID_MSG_SFOILS_CLOSED_1: return "\03S-Foils han llegado a la posición [cerrada]";
		case ID_MSG_SFOILS_OPEN_1: return "\03S-Foils han alcanzado la posición [abierta]";
		case ID_MSG_SFOILS_NO_FIRE: return "\03Los cañones no pueden disparar con las aletas S cerradas o los trenes abiertos";
		case ID_MSG_SFOILS_CLOSING_2: return "\03Apertura de los trenes";
		case ID_MSG_SFOILS_OPENING_2: return "\03Cierre de los trenes ";
		case ID_MSG_SFOILS_CLOSED_2: return "\03Los trenes han alcanzado la posición [abierta]";
		case ID_MSG_SFOILS_OPEN_2: return "\03Los trenes han alcanzado la posición [cerrada].";
		case ID_MSG_NOT_EQUIPPED_SFOIL_1: return "\03Su embarcación no está equipada con [S-Foils]";
		case ID_MSG_NOT_EQUIPPED_SFOIL_2: return "\03[Landing Gears] ya están abriendo o cerrando";
		case ID_MSG_NOT_EQUIPPED_SFOIL_3: return "\03Cerrar [Landing Gear] antes de activar [S-Foils]";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_1: return "\03Su embarcación no está equipada con [Landing Gears]";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_2: return "\03[S-Foils] ya están abriendo o cerrando";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_3: return "\03Cerrar  [S-Foils] antes de activar [Landing Gears]";
		case ID_MSG_HANGAR_TRACTOR_1: return "\04Cierre los S-Foils y abra los trenes antes de entrar en el hanga";
		case ID_MSG_HANGAR_TRACTOR_2: return "\04Cierre los S-Foils antes de entrar en el hangar";
		case ID_MSG_HANGAR_TRACTOR_3: return "\04Abrir los trenes antes de entrar en el hangar";
		case ID_MSG_HANGAR_TRACTOR_4: return "\04Pulse [Space] para activar el rayo tractor y entrar en el hangar";
		case ID_MSG_ASK_ABOUT_HYPER_1: return "\03Cerrar los trenes antes de entrar en el hiperespacio";
		case ID_MSG_ASK_ABOUT_HYPER_2: return "\03Pulse [SPACE] al hiperespacio a [*]";
		}
	}
	else if (language == "fr")
	{
		switch (id)
		{
		case ID_MSG_SFOILS_CLOSING_1: return "\03Fermeture des S-Foils";
		case ID_MSG_SFOILS_OPENING_1: return "\03Ouverture des S-Foils";
		case ID_MSG_SFOILS_CLOSED_1: return "\03S-Foils ont atteint la position [fermée]";
		case ID_MSG_SFOILS_OPEN_1: return "\03S-Foils ont atteint la position [ouverte]";
		case ID_MSG_SFOILS_NO_FIRE: return "\03Les canons ne peuvent pas tirer avec les S-Foils fermés ou les trains ouverts";
		case ID_MSG_SFOILS_CLOSING_2: return "\03Ouverture des trains d'atterrissage";
		case ID_MSG_SFOILS_OPENING_2: return "\03Fermeture des trains d'atterrissage";
		case ID_MSG_SFOILS_CLOSED_2: return "\03Les trains ont atteint la position [ouverte].";
		case ID_MSG_SFOILS_OPEN_2: return "\03Les trains ont atteint la position [fermée]";
		case ID_MSG_NOT_EQUIPPED_SFOIL_1: return "\03Votre vaisseau n'est pas équipé de [S-Foils]";
		case ID_MSG_NOT_EQUIPPED_SFOIL_2: return "\03[Landing Gears] sont déjà ouverts ou fermés";
		case ID_MSG_NOT_EQUIPPED_SFOIL_3: return "\03Fermer [Landing Gear] avant d'activer [S-Foils]";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_1: return "\03Votre vaisseau n'est pas équipé de [Landing Gears]";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_2: return "\03[S-Foils] sont déjà ouverts ou fermés";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_3: return "\03Fermer [S-Foils] avant d'active [Landing Gears]";
		case ID_MSG_HANGAR_TRACTOR_1: return "\04Fermez les S-Foils et ouvrez les trains avant d'entrer dans le hangar";
		case ID_MSG_HANGAR_TRACTOR_2: return "\04Fermer les S-Foils avant d'entrer dans le hangar";
		case ID_MSG_HANGAR_TRACTOR_3: return "\04Ouvrir les trains avant d'entrer dans le hangar";
		case ID_MSG_HANGAR_TRACTOR_4: return "\04Presse [Space] pour activer le rayon tracteur et entrer dans le hangar";
		case ID_MSG_ASK_ABOUT_HYPER_1: return "\03Fermez les trains avant d'entrer dans l'hyperespace";
		case ID_MSG_ASK_ABOUT_HYPER_2: return "\03Appuyez sur la touche [SPACE] pour passer en hyperespace. [*]";
		}
	}
	else if (language == "de")
	{
		switch (id)
		{
		case ID_MSG_SFOILS_CLOSING_1: return "\03S-Foils schließen";
		case ID_MSG_SFOILS_OPENING_1: return "\03S-Foils Öffnen";
		case ID_MSG_SFOILS_CLOSED_1: return "\03S-Foils haben die [geschlossene] Position erreicht";
		case ID_MSG_SFOILS_OPEN_1: return "\03S-Foils haben [offene] Position erreicht";
		case ID_MSG_SFOILS_NO_FIRE: return "\03Kanonen können nicht feuern, wenn die S-Foils geschlossen oder die Fahrwerke geöffnet sind";
		case ID_MSG_SFOILS_CLOSING_2: return "\03Fahrwerke öffnen";
		case ID_MSG_SFOILS_OPENING_2: return "\03Fahrwerke schließen";
		case ID_MSG_SFOILS_CLOSED_2: return "\03Fahrwerke haben die Position [offen] erreicht";
		case ID_MSG_SFOILS_OPEN_2: return "\03Fahrwerke haben die Position [geschlossen] erreicht";
		case ID_MSG_NOT_EQUIPPED_SFOIL_1: return "\03Ihr Fahrzeug ist nicht ausgestattet mit [S-Foils]";
		case ID_MSG_NOT_EQUIPPED_SFOIL_2: return "\03[Landing Gears] werden bereits geöffnet oder geschlossen";
		case ID_MSG_NOT_EQUIPPED_SFOIL_3: return "\03Schließen Sie [Fahrwerk] vor der Aktivierung [S-Foils]";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_1: return "\03Ihr Fahrzeug ist nicht ausgestattet mit [Landing Gears]";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_2: return "\03[S-Foils] sind bereits geöffnet oder geschlossen";
		case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_3: return "\03Schließen [S-Foils] vor aktivieren [Landing Gears]";
		case ID_MSG_HANGAR_TRACTOR_1: return "\04Close S-Foils und Fahrwerk öffnen, bevor Sie den Hangar betreten";
		case ID_MSG_HANGAR_TRACTOR_2: return "\04Schließen S-Foils vor Betreten des Hangars";
		case ID_MSG_HANGAR_TRACTOR_3: return "\04Öffnen Sie Fahrwerke vor Betreten des Hangars";
		case ID_MSG_HANGAR_TRACTOR_4: return "\04Drücken Sie die [Leertaste], um den Traktorstrahl zu aktivieren und den Hangar zu betreten";
		case ID_MSG_ASK_ABOUT_HYPER_1: return "\03Fahrwerk vor Eintritt in den Hyperraum schließen";
		case ID_MSG_ASK_ABOUT_HYPER_2: return "\03Drücken Sie die [Leertaste], um in den Hyperraum zu [*]";
		}
	}

	switch (id)
	{
	case ID_MSG_SFOILS_CLOSING_1: return "\03S-Foils closing";
	case ID_MSG_SFOILS_OPENING_1: return "\03S-Foils opening";
	case ID_MSG_SFOILS_CLOSED_1: return "\03S-Foils have reached [closed] position";
	case ID_MSG_SFOILS_OPEN_1: return "\03S-Foils have reached [open] position";
	case ID_MSG_SFOILS_NO_FIRE: return "\03Cannons cannot fire with S-Foils closed or Landing Gears opened";
	case ID_MSG_SFOILS_CLOSING_2: return "\03Landing Gears opening";
	case ID_MSG_SFOILS_OPENING_2: return "\03Landing Gears closing";
	case ID_MSG_SFOILS_CLOSED_2: return "\03Landing Gears have reached [open] position";
	case ID_MSG_SFOILS_OPEN_2: return "\03Landing Gears have reached [closed] position";
	case ID_MSG_NOT_EQUIPPED_SFOIL_1: return "\03Your craft is not equipped with [S-Foils]";
	case ID_MSG_NOT_EQUIPPED_SFOIL_2: return "\03[Landing Gears] are already opening or closing";
	case ID_MSG_NOT_EQUIPPED_SFOIL_3: return "\03Close [Landing Gear] before engaging [S-Foils]";
	case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_1: return "\03Your craft is not equipped with [Landing Gears]";
	case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_2: return "\03[S-Foils] are already opening or closing";
	case ID_MSG_NOT_EQUIPPED_LANDINGGEARS_3: return "\03Close [S-Foils] before engaging [Landing Gears]";
	case ID_MSG_HANGAR_TRACTOR_1: return "\04Close S-Foils and open Landing Gears before enter hangar";
	case ID_MSG_HANGAR_TRACTOR_2: return "\04Close S-Foils before enter hangar";
	case ID_MSG_HANGAR_TRACTOR_3: return "\04Open Landing Gears before enter hangar";
	case ID_MSG_HANGAR_TRACTOR_4: return "\04Hit [Space] to activate tractor beam and enter hangar";
	case ID_MSG_ASK_ABOUT_HYPER_1: return "\03Close Landing Gears before enter hyperspace";
	case ID_MSG_ASK_ABOUT_HYPER_2: return "\03Press [SPACE] to hyperspace to [*]";
	}

	return "";
}
