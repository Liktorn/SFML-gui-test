#pragma once
#include <string>
namespace bw {

	class Monster
	{
	public:
		Monster() : value(0) {};

		std::string text[24]{
			u8"Asgnagare"
			, u8"Betesdjur"
			, u8"Dr�partr�d"
			, u8"Dr�mblomster"
			, u8"Etterbestar"
			, u8"Fr�targr�s"
			, u8"Jordslukare"
			, u8"Magtorsk(smittat vatten)"
			, u8"Maskinvarelse"
			, u8"Masksv�rm"
			, u8"Parasitsvamp(smittad varelse)"
			, u8"Psi - mygg"
			, u8"Raggbest"
			, u8"Ruinspindel"
			, u8"R�tmyror"
			, u8"Skr�desvr�k"
			, u8"Sk�vlare"
			, u8"Skrotkr�kor"
			, u8"Strykarhundar"
			, u8"Tj�rnmal"
			, u8"Zongetingar"
			, u8"Zoniglar"
			, u8"Zont�rne"
			, u8"�rgflyn"
		};
		int range[2][24]{
			{ 11,13,15,16,21,23,25,31,32,33,34,35,36,42,44,46,51,52,54,56,61,63,65,66 }
			,{ 12,14,15,16,22,24,26,31,32,33,34,35,41,43,45,56,51,53,55,56,62,64,65,66}
		};
		int value;
	};

} //namespace