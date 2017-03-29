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
// Hur många hot och artefakter
int randHot = 0;
int randArtefakt = 0;


// Sätter miljö alternativen
std::string miljoStr[]{ u8"11-12\tTät skog" , u8"13-15\tBusklandskap", u8"16-21\tTräsk", u8"22-24\tDöd skog"
	, u8"25-26\tAsköken", u8"31   \tEnorm krater", u8"32   \tFält av glas", u8"33-35\tÖvervuxna ruiner"
	, u8"36-42\tSöndervittrade ruiner", u8"43-51\tFörfallna ruiner", u8"52-56\tVälbevarade ruiner"
	, u8"61-64\tÖde industrilandskap", u8"65-66\tBosättning" };
static int miljoValue = 0;

// sätter standard ruin alternativen
std::string ruinStandardStr[]{
	u8"11. Affärsgalleria"
	, u8"12. Badhus"
	, u8"13. Bensinstation"
	, u8"14. Biograf"
	, u8"15. Bostadsområde"
	, u8"16. Busstation"
	, u8"21. Flygplansvrak"
	, u8"22. Förvildad park"
	, u8"23. Höghusruin"
	, u8"24. Jaktaffär"
	, u8"25. Kontorskomplex"
	, u8"26. Krater"
	, u8"31. Kråkslott"
	, u8"32. Kyrka"
	, u8"33. Lekplats"
	, u8"34. Miljonprogram"
	, u8"35. Motorväg"
	, u8"36. Museum"
	, u8"41. Nöjesfält"
	, u8"42. Parkeringshus"
	, u8"43. Polisstation"
	, u8"44. Radiostation"
	, u8"45. Raserad bro"
	, u8"46. Skyddsrum"
	, u8"51. Slagfält"
	, u8"52. Snabbmatsrestaurang"
	, u8"53. Sporthall"
	, u8"54. Sjukhus"
	, u8"55. Stridsvagn"
	, u8"56. Teater"
	, u8"61. Tunnelbana"
	, u8"62. Tågstation"
	, u8"63. Snabbköp"
	, u8"64. Vägtunnel"
	, u8"65. Övergiven skola"
	, u8"66. Ödelagd marina"
};
static int ruinStandardValue = 0;

// sätter industriruin alternativen
std::string ruinIndustriStr[]{
	u8"11-13.\tFabrik"
	, u8"14-16.\tFartygsvrak"
	, u8"21-23.\tKraftledning"
	, u8"24-26.\tMilitärbas"
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

// sätter Rötnivå
std::string rotStr[]{
	u8"1.\  t0 röt-oas. RP tar inga rötpoäng."
	, u8"2-5.\t1 typisk zonsektor. RP tar en rötpoäng varje dygn."
	, u8"6.  \t2 extra rötdrabbad sektor. En rötpoäng per timme."
};
static int rotValue = 0;

// sätter hotnivå
std::string hotNiva[]{
	u8"1. Zonens ytterområden"
	, u8"2. Zonens ytterområden"
	, u8"3. Zonens ytterområden"
	, u8"4. Zonens ytterområden"
	, u8"5. Zonens centrala delar"
	, u8"6. Zonens centrala delar"
	, u8"7. Zonens centrala delar"
	, u8"8. Zonens centrala delar"
	, u8"9 + Särskilt farliga sektorer"
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
		"\nResten av kapitlet beskriver hur du steg för steg skapar en sektor i Zonen."
		"Du kan välja fritt ur tabellerna eller rulla T66."
		" Följ dessa steg när RP går in i en ny sektor :"
		"\n1. Bestäm allmän sektormiljö."
		"\n2. Bestäm en dominerande ruin i sektorn."
		"\n3. Avgör rötnivån."
		"\n4. Avgör hotnivå och slå hotslaget."
		"\n5. Om det finns ett hot i sektorn, avgör vilket."
		"\n6. Om det finns en artefakt i sektorn, avgör vilken."
		"\n7. Om det finns en zonstrykare i gruppen, låt honom slå för att leda vägen (se kapitel 3 i Regelboken)."
		"\n8. Om du vill, slå eller välj en detalj att använda i din beskrivning.");

	if (ImGui::Button(u8"Fortsätt", ImVec2(50, 50)))
	{
		skapa_sektor = false;
		sektor_miljo = true;
	}; 
}
void sektorMiljo()
{
	ImGui::TextWrapped(u8"SEKTORMILJÖ\n"
		"Börja med att etablera den allmänna miljön\n"
		"i sektorn.Slå T66 eller välj : ");

	int size = ARRAY_SIZE(miljoStr);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&miljoStr[i][0], &miljoValue, i);

	ImGui::TextWrapped(u8"\nVälj en av ovan eller slumpa med hjälp av knappen."
		"\nTryck på fortsätt för att gå vidare");
	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ImVec2(100, 30)))
	{
		miljoValue = rand() % size;
	}; ImGui::SameLine();
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ImVec2(100, 30)))
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
	ImGui::Text(u8"Vald miljö är: ");
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
		"Om sektormiljön är delvis raserade ruiner, välbevarade ruiner eller industrilandskap "
		"är nästa steg att avgöra vilken ruin som dominerar sektorn.Det finns "
		"förstås mer än en ruin i varje sektor, men	den utvalda ruinen är den som väcker "
		"RP : s intresse.");
	if (ruin_industri)
	{
		int size = ARRAY_SIZE(ruinIndustriStr);

		for (int i = 0; i < size; i++)
			ImGui::RadioButton(&ruinIndustriStr[i][0], &ruinIndustriValue, i);

		ImGui::TextWrapped(u8"\nVälj en av ovan eller slumpa med hjälp av knappen."
			"\nTryck på fortsätt för att gå vidare");
		// SLUMPA KNAPP
		if (ImGui::Button(u8"Slumpa värde", ImVec2(100, 30)))
		{
			ruinIndustriValue = rand() % size;
		}; ImGui::SameLine();
		// NÄSTA KNAPP
		if (ImGui::Button(u8"Fortsätt", ImVec2(100, 30)))
		{
			sektor_ruin = false;
			sektor_rota = true;
			rot();
		}
		ImGui::Text(u8"Vald ruin är: ");
		ImGui::SameLine(); ImGui::Text(&ruinIndustriStr[ruinIndustriValue][0]);
	}
	else if (ruin_standard)
	{
		int size = ARRAY_SIZE(ruinStandardStr);

		for (int i = 0; i < size; i++)
			ImGui::RadioButton(&ruinStandardStr[i][0], &ruinStandardValue, i);
		// SLUMPA KNAPP
		if (ImGui::Button(u8"Slumpa värde", ImVec2(100, 30)))
		{
			ruinStandardValue = rand() % size;
		}; ImGui::SameLine();
		// NÄSTA KNAPP
		if (ImGui::Button(u8"Fortsätt", ImVec2(100, 30)))
		{
			sektor_ruin = false;
			sektor_rota = true;
			rot();
		}
		ImGui::Text(u8"Vald ruin är: ");
		ImGui::SameLine(); ImGui::Text(&ruinStandardStr[ruinStandardValue][0]);
	}
	
}

void rot()
{
	ImGui::TextWrapped(u8"RÖTNIVÅ\n"
		"Välj eller slå fram rötnivån i sektorn, en siffra från 0 till 3. 1 eller 2 är det vanliga "
		"i Zonen.Läs mer om zonröta i kapitel 8 i Regelboken. "
		"Om du har en dold sl - karta kan du rita in på förhand vilken rötnivå Zonens sektorer har.");

	int size = ARRAY_SIZE(rotStr);

	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&rotStr[i][0], &rotValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ImVec2(100, 30)))
	{
			int randomValue = rand() % 6 + 1;
			if (randomValue == 1)
				rotValue = 0;
			else if (randomValue == 6)
				rotValue = 2;
			else
				rotValue = 1;
	}; ImGui::SameLine();
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ImVec2(100, 30)))
	{
		sektor_rota = false;
		sektor_hot = true;
		hotMeny();
	}

	ImGui::Text(u8"Vald röta är: ");
	ImGui::SameLine(); ImGui::Text(&rotStr[rotValue][0]);
}

void hotMeny()
{
	ImGui::TextWrapped(u8"Nästa steg är att bestämma hotnivån.\n"
		"Hotnivån är ett värde från noll och uppåt. "
		"Ett högt värde betyder en farligare sektor, men också större chans att "
		"det finns orörda artefakter där.\n"
		"Om sektormiljön (ovan) är bosättning har sektorn "
		"ingen hotnivå.Gå direkt till steg 5 "
		"om bosättningar, nedan.");

	int size = ARRAY_SIZE(hotNiva);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&hotNiva[i][0], &hotValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ImVec2(100, 30)))
	{
		hotValue = rand() % size;

	}; ImGui::SameLine();
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ImVec2(100, 30)))
	{
		sektor_hot = false;
		sektor_random_hot = true;
		randomHot();
	}
	ImGui::Text(u8"Vald hotnivå är: ");
	ImGui::SameLine(); ImGui::Text(&hotNiva[hotValue][0]);
}
void randomHot()
{
	ImGui::TextWrapped(u8"När rollpersonerna går in i sektorn rullar du ett antal grundtärningar lika "
		"med hotnivån.Varje RADIAK betyder att det finns en artefakt i sektorn.Slår du minst "
		"en BIOHAZARD betyder det att det finns ett hot i sektorn, och ju fler BIOHAZARD du slår, desto "
		"värre är hotet.Se steg 5 nedan.\n"
		"Om sektormiljön är skog eller totalt raserade ruiner(ovan) finns det inga artefakter i sektorn, oavsett vad tärningarna visar. "
		"RADIAK har alltså ingen betydelse.\n"
		"Nattetid är det farligt att röra sig i Zonen.Räkna hotnivån som tre steg högre än normalt.");

	static int e = 0;

	ImGui::RadioButton("Dagtid", &e, 0);
	ImGui::RadioButton("Nattetid", &e, 1);

	int tempHot = hotValue + (e * 3) + 1;
	

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ImVec2(100, 30)))
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
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ImVec2(100, 30)))
	{
		hotValue = tempHot;
		sektor_random_hot = false;
		sektor_hot_i_zonen = true;
		hotIZonen();

	}
	ImGui::Text(u8"Hotnivån är: %d", tempHot);
	if (!hot)
		ImGui::Text(u8"Inga hot påträffas på grund av zonvalen");
	else
		ImGui::Text(u8"Antal hot är: %d", randHot);
	if (!artefakt)
		ImGui::Text(u8"Inga artefakter hittar på grund av zonvalen");
	else
		ImGui::Text(u8"Antal artefakter är: %d", randArtefakt);
}

void hotIZonen()
{
	ImGui::TextWrapped(u8"Hot i Zonen tar sig tre olika former : "
		"Humanoider, monster och fenomen.Läs mer om dem i kapitel 5. Slå en T6 eller välj : ");

	for (int i = 0; i < ARRAY_SIZE(hotIZonenStr); i++)
		ImGui::RadioButton(&hotIZonenStr[i][0], &hotIZonenValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ImVec2(100, 30)))
	{
		int temp = rand() % 6 + 1;
		if (temp == 1 || temp == 2)
			hotIZonenValue = 0;
		else if (temp == 6)
			hotIZonenValue = 2;
		else
			hotIZonenValue = 1;

	}; ImGui::SameLine();
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ImVec2(100, 30)))
	{

		sektor_hot_i_zonen = false;
		hotIZonen();

	}
}