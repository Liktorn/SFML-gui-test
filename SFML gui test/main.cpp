#include "imgui.h"
#include "imgui-sfml.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <time.h>
#include <string>
#include <vector>

#include "header.h"


#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

// Show text bools.
bool skapa_sektor = true;
bool sektor_miljo = false;
bool sektor_ruin = false;
bool sektor_rota = false;
bool sektor_hot = false;
bool sektor_random_hot = false;
bool sektor_hot_i_zonen = false;
bool humanoider = false;

// Om det skall finnas hot, ruiner och artefakter i zonen
bool ruin_standard = false;
bool ruin_industri = false;
bool hot = false;
bool artefakt = false;
// Hur m�nga hot och artefakter
int randHot = 0;
int randArtefakt = 0;


// S�tter milj� alternativen
std::string miljoStr[]{ u8"11-12\tT�t skog" , u8"13-15\tBusklandskap", u8"16-21\tTr�sk", u8"22-24\tD�d skog"
	, u8"25-26\tAsk�ken", u8"31   \tEnorm krater", u8"32   \tF�lt av glas", u8"33-35\t�vervuxna ruiner"
	, u8"36-42\tS�ndervittrade ruiner", u8"43-51\tF�rfallna ruiner", u8"52-56\tV�lbevarade ruiner"
	, u8"61-64\t�de industrilandskap", u8"65-66\tBos�ttning" };
static int miljoValue = 0;

// s�tter standard ruin alternativen
std::string ruinStandardStr[]{
	u8"11. Aff�rsgalleria"
	, u8"12. Badhus"
	, u8"13. Bensinstation"
	, u8"14. Biograf"
	, u8"15. Bostadsomr�de"
	, u8"16. Busstation"
	, u8"21. Flygplansvrak"
	, u8"22. F�rvildad park"
	, u8"23. H�ghusruin"
	, u8"24. Jaktaff�r"
	, u8"25. Kontorskomplex"
	, u8"26. Krater"
	, u8"31. Kr�kslott"
	, u8"32. Kyrka"
	, u8"33. Lekplats"
	, u8"34. Miljonprogram"
	, u8"35. Motorv�g"
	, u8"36. Museum"
	, u8"41. N�jesf�lt"
	, u8"42. Parkeringshus"
	, u8"43. Polisstation"
	, u8"44. Radiostation"
	, u8"45. Raserad bro"
	, u8"46. Skyddsrum"
	, u8"51. Slagf�lt"
	, u8"52. Snabbmatsrestaurang"
	, u8"53. Sporthall"
	, u8"54. Sjukhus"
	, u8"55. Stridsvagn"
	, u8"56. Teater"
	, u8"61. Tunnelbana"
	, u8"62. T�gstation"
	, u8"63. Snabbk�p"
	, u8"64. V�gtunnel"
	, u8"65. �vergiven skola"
	, u8"66. �delagd marina"
};
static int ruinStandardValue = 0;

// s�tter industriruin alternativen
std::string ruinIndustriStr[]{
	u8"11-13.\tFabrik"
	, u8"14-16.\tFartygsvrak"
	, u8"21-23.\tKraftledning"
	, u8"24-26.\tMilit�rbas"
	, u8"31-33.\tPipeline"
	, u8"34-36.\tRadiomast"
	, u8"41-43.\tRaffinaderi"
	, u8"44-46.\tReningsverk"
	, u8"51-53.\tSkjutbana"
	, u8"54-56.\tSoptipp"
	, u8"61-63.\tVindkraftverk"
	, u8"64-66.\tOljecistern"
};
static int ruinIndustriValue = 0;

// s�tter R�tniv�
std::string rotStr[]{
	u8"1.\  t0 r�t-oas. RP tar inga r�tpo�ng."
	, u8"2-5.\t1 typisk zonsektor. RP tar en r�tpo�ng varje dygn."
	, u8"6.  \t2 extra r�tdrabbad sektor. En r�tpo�ng per timme."
};
static int rotValue = 0;

// s�tter hotniv�
std::string hotNiva[]{
	u8"1. Zonens ytteromr�den"
	, u8"2. Zonens ytteromr�den"
	, u8"3. Zonens ytteromr�den"
	, u8"4. Zonens ytteromr�den"
	, u8"5. Zonens centrala delar"
	, u8"6. Zonens centrala delar"
	, u8"7. Zonens centrala delar"
	, u8"8. Zonens centrala delar"
	, u8"9 + S�rskilt farliga sektorer"
};
static int hotValue = 0;

// hot i zonen
std::string hotIZonenStr[]{
	u8"1 - 2. Humanoider"
	, u8"3 - 5. Monster"
	, u8"6. Fenomen"
};
static int hotIZonenValue = 0;

// hot i zonen - humanoider
std::string humanoiderStr[]{
	u8""
};
static int humanoiderValue = 0;
int main()
{
	static int HEIGHT = 600;
	static int WIDTH = 800;

	srand((unsigned int)time(NULL));

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	sf::Color bgColor;

	float color[3] = { 0.f, 0.f, 0.f };

	// let's use char array as buffer, see next part
	// for instructions on using std::string with ImGui
	char windowTitle[255] = "ImGui + SFML = <3";

	window.setTitle(windowTitle);
	window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	sf::Clock deltaClock;


	

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		// Setting next window to full screen and with a static size and position
		ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
		ImGui::SetNextWindowSizeConstraints(ImVec2(WIDTH, HEIGHT), ImVec2(WIDTH, HEIGHT));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin("Skapa Zonsektorer"); // begin window

		/*skapa_sektor = false;
		sektor_hot_i_zonen = true;*/

		if (skapa_sektor)
			inledning();
		else if (sektor_miljo)
			sektorMiljo();
		else if (sektor_ruin)
			sektorRuin();
		else if (sektor_rota)
			rot();
		else if (sektor_hot)
			hotMeny();
		else if (sektor_random_hot)
			randomHot();
		else if (sektor_hot_i_zonen)
			hotIZonen();



		
	
		ImGui::End(); // end window




	/*	static bool selected[4] = { false, true, false, false };
		ImGui::Selectable("1. I am selectable", &selected[0]);
		ImGui::Selectable("2. I am selectable", &selected[1]);
		ImGui::Text("3. I am not selectable");
		ImGui::Selectable("4. I am selectable", &selected[2]);*/





		
		//ImGui::ShowTestWindow();



		window.clear(bgColor); // fill background with color
		ImGui::Render();
		window.display();
	}

	ImGui::SFML::Shutdown();
}


void inledning()
{
	ImGui::TextWrapped(u8"SKAPA SEKTOR"
		"\nResten av kapitlet beskriver hur du steg f�r steg skapar en sektor i Zonen."
		"Du kan v�lja fritt ur tabellerna eller rulla T66."
		" F�lj dessa steg n�r RP g�r in i en ny sektor :"
		"\n1. Best�m allm�n sektormilj�."
		"\n2. Best�m en dominerande ruin i sektorn."
		"\n3. Avg�r r�tniv�n."
		"\n4. Avg�r hotniv� och sl� hotslaget."
		"\n5. Om det finns ett hot i sektorn, avg�r vilket."
		"\n6. Om det finns en artefakt i sektorn, avg�r vilken."
		"\n7. Om det finns en zonstrykare i gruppen, l�t honom sl� f�r att leda v�gen (se kapitel 3 i Regelboken)."
		"\n8. Om du vill, sl� eller v�lj en detalj att anv�nda i din beskrivning.");

	if (ImGui::Button(u8"Forts�tt", ImVec2(50, 50)))
	{
		skapa_sektor = false;
		sektor_miljo = true;
	}; 
}
void sektorMiljo()
{
	ImGui::TextWrapped(u8"SEKTORMILJ�\n"
		"B�rja med att etablera den allm�nna milj�n\n"
		"i sektorn.Sl� T66 eller v�lj : ");

	int size = ARRAY_SIZE(miljoStr);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&miljoStr[i][0], &miljoValue, i);

	ImGui::TextWrapped(u8"\nV�lj en av ovan eller slumpa med hj�lp av knappen."
		"\nTryck p� forts�tt f�r att g� vidare");
	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ImVec2(100, 30)))
	{
		miljoValue = rand() % size;
	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ImVec2(100, 30)))
	{
		sektor_miljo = false;
		if (ruin_industri || ruin_standard)
		{
			sektor_ruin = true;
			sektorRuin();
		}
		else
		{
			sektor_rota = true;
			rot();
		}

	}
	ImGui::Text(u8"Vald milj� �r: ");
	ImGui::SameLine(); ImGui::Text(&miljoStr[miljoValue][0]);

	// kollar om det skall finnas hot, ruiner och artefakter
	if (miljoValue >= 0 && miljoValue <= 6 || miljoValue == 12)
	{
		ruin_industri = false;
		ruin_standard = false;
	}
	else if (miljoValue <= 11 && miljoValue != 12)
	{
		if (miljoValue >= 7 && miljoValue <= 10)
		{
			ruin_industri = false;
			ruin_standard = true;
		}
		else if (miljoValue == 11)
		{
			ruin_industri = true;
			ruin_standard = false;
		}
	}
	if (miljoValue < 12)
		hot = true;
	else
		hot = false;
	if (miljoValue <= 6 || miljoValue == 12)
		artefakt = false;
	else
		artefakt = true;

}
void sektorRuin()
{
	ImGui::TextWrapped(u8"RUINER\n"
		"Om sektormilj�n �r delvis raserade ruiner, v�lbevarade ruiner eller industrilandskap "
		"�r n�sta steg att avg�ra vilken ruin som dominerar sektorn.Det finns "
		"f�rst�s mer �n en ruin i varje sektor, men	den utvalda ruinen �r den som v�cker "
		"RP : s intresse.");
	if (ruin_industri)
	{
		int size = ARRAY_SIZE(ruinIndustriStr);

		for (int i = 0; i < size; i++)
			ImGui::RadioButton(&ruinIndustriStr[i][0], &ruinIndustriValue, i);

		ImGui::TextWrapped(u8"\nV�lj en av ovan eller slumpa med hj�lp av knappen."
			"\nTryck p� forts�tt f�r att g� vidare");
		// SLUMPA KNAPP
		if (ImGui::Button(u8"Slumpa v�rde", ImVec2(100, 30)))
		{
			ruinIndustriValue = rand() % size;
		}; ImGui::SameLine();
		// N�STA KNAPP
		if (ImGui::Button(u8"Forts�tt", ImVec2(100, 30)))
		{
			sektor_ruin = false;
			sektor_rota = true;
			rot();
		}
		ImGui::Text(u8"Vald ruin �r: ");
		ImGui::SameLine(); ImGui::Text(&ruinIndustriStr[ruinIndustriValue][0]);
	}
	else if (ruin_standard)
	{
		int size = ARRAY_SIZE(ruinStandardStr);

		for (int i = 0; i < size; i++)
			ImGui::RadioButton(&ruinStandardStr[i][0], &ruinStandardValue, i);
		// SLUMPA KNAPP
		if (ImGui::Button(u8"Slumpa v�rde", ImVec2(100, 30)))
		{
			ruinStandardValue = rand() % size;
		}; ImGui::SameLine();
		// N�STA KNAPP
		if (ImGui::Button(u8"Forts�tt", ImVec2(100, 30)))
		{
			sektor_ruin = false;
			sektor_rota = true;
			rot();
		}
		ImGui::Text(u8"Vald ruin �r: ");
		ImGui::SameLine(); ImGui::Text(&ruinStandardStr[ruinStandardValue][0]);
	}
	
}

void rot()
{
	ImGui::TextWrapped(u8"R�TNIV�\n"
		"V�lj eller sl� fram r�tniv�n i sektorn, en siffra fr�n 0 till 3. 1 eller 2 �r det vanliga "
		"i Zonen.L�s mer om zonr�ta i kapitel 8 i Regelboken. "
		"Om du har en dold sl - karta kan du rita in p� f�rhand vilken r�tniv� Zonens sektorer har.");

	int size = ARRAY_SIZE(rotStr);

	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&rotStr[i][0], &rotValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ImVec2(100, 30)))
	{
			int randomValue = rand() % 6 + 1;
			if (randomValue == 1)
				rotValue = 0;
			else if (randomValue == 6)
				rotValue = 2;
			else
				rotValue = 1;
	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ImVec2(100, 30)))
	{
		sektor_rota = false;
		sektor_hot = true;
		hotMeny();
	}

	ImGui::Text(u8"Vald r�ta �r: ");
	ImGui::SameLine(); ImGui::Text(&rotStr[rotValue][0]);
}

void hotMeny()
{
	ImGui::TextWrapped(u8"N�sta steg �r att best�mma hotniv�n.\n"
		"Hotniv�n �r ett v�rde fr�n noll och upp�t. "
		"Ett h�gt v�rde betyder en farligare sektor, men ocks� st�rre chans att "
		"det finns or�rda artefakter d�r.\n"
		"Om sektormilj�n (ovan) �r bos�ttning har sektorn "
		"ingen hotniv�.G� direkt till steg 5 "
		"om bos�ttningar, nedan.");

	int size = ARRAY_SIZE(hotNiva);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&hotNiva[i][0], &hotValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ImVec2(100, 30)))
	{
		hotValue = rand() % size;

	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ImVec2(100, 30)))
	{
		sektor_hot = false;
		sektor_random_hot = true;
		randomHot();
	}
	ImGui::Text(u8"Vald hotniv� �r: ");
	ImGui::SameLine(); ImGui::Text(&hotNiva[hotValue][0]);
}
void randomHot()
{
	ImGui::TextWrapped(u8"N�r rollpersonerna g�r in i sektorn rullar du ett antal grundt�rningar lika "
		"med hotniv�n.Varje RADIAK betyder att det finns en artefakt i sektorn.Sl�r du minst "
		"en BIOHAZARD betyder det att det finns ett hot i sektorn, och ju fler BIOHAZARD du sl�r, desto "
		"v�rre �r hotet.Se steg 5 nedan.\n"
		"Om sektormilj�n �r skog eller totalt raserade ruiner(ovan) finns det inga artefakter i sektorn, oavsett vad t�rningarna visar. "
		"RADIAK har allts� ingen betydelse.\n"
		"Nattetid �r det farligt att r�ra sig i Zonen.R�kna hotniv�n som tre steg h�gre �n normalt.");

	static int e = 0;

	ImGui::RadioButton("Dagtid", &e, 0);
	ImGui::RadioButton("Nattetid", &e, 1);

	int tempHot = hotValue + (e * 3) + 1;
	

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ImVec2(100, 30)))
	{
		randArtefakt = 0;
		randHot = 0;
		for (int i = 0; i < tempHot; i++)
		{
			int tempRand = rand() % 6 + 1;
			if (tempRand == 1)
				randHot++;
			else if (tempRand == 6)
				randArtefakt++;
			std::cout << tempRand << std::endl;

		}
		

	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ImVec2(100, 30)))
	{
		hotValue = tempHot;
		sektor_random_hot = false;
		sektor_hot_i_zonen = true;
		hotIZonen();

	}
	ImGui::Text(u8"Hotniv�n �r: %d", tempHot);
	if (!hot)
		ImGui::Text(u8"Inga hot p�tr�ffas p� grund av zonvalen");
	else
		ImGui::Text(u8"Antal hot �r: %d", randHot);
	if (!artefakt)
		ImGui::Text(u8"Inga artefakter hittar p� grund av zonvalen");
	else
		ImGui::Text(u8"Antal artefakter �r: %d", randArtefakt);
}

void hotIZonen()
{
	ImGui::TextWrapped(u8"Hot i Zonen tar sig tre olika former : "
		"Humanoider, monster och fenomen.L�s mer om dem i kapitel 5. Sl� en T6 eller v�lj : ");

	for (int i = 0; i < ARRAY_SIZE(hotIZonenStr); i++)
		ImGui::RadioButton(&hotIZonenStr[i][0], &hotIZonenValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ImVec2(100, 30)))
	{
		int temp = rand() % 6 + 1;
		if (temp == 1 || temp == 2)
			hotIZonenValue = 0;
		else if (temp == 6)
			hotIZonenValue = 2;
		else
			hotIZonenValue = 1;

	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ImVec2(100, 30)))
	{

		sektor_hot_i_zonen = false;
		hotIZonen();

	}
}