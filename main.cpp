#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
using namespace std;
using namespace sf;
//Funktio, joka tarkistaa että, onko kumpikaan pelaajista voittanut.
int voitto(int r[3][3])
{
	//Tarkista kummatkin vinosuorat, jos jomman kumman kaikki ruudut ovat saman pelaajan, palauta 1 tai 2, riippuen siitä kumpi voitti.
	//Vinosuora 1:
	if(r[0][0] == r[1][1] && r[1][1] == r[2][2]) return r[0][0];
	//Vinosuora 2:
	if(r[0][2] == r[1][1] && r[1][1] == r[2][0]) return r[0][2];
	//Tarkista kaikki pysty ja vaakarivit:
	//Jos kaikki rivin ruudut ovat saman pelaajan, palauta voittaneen pelaajan numero 1 tai 2.
	//Tässä voisi käyttää for silmukkaa, mutta ruudukko on 3*3 joten if lauseiden käyttö on mielestäni hyväksyttävää.
	if(r[0][0] == r[0][1] && r[0][1] == r[0][2] && r[0][0] != 0) return r[0][0];
	if(r[1][0] == r[1][1] && r[1][1] == r[1][2] && r[1][0] != 0) return r[1][0];
	if(r[2][0] == r[2][1] && r[2][1] == r[2][2] && r[2][0] != 0) return r[2][2];
	if(r[0][0] == r[1][0] && r[1][0] == r[2][0] && r[0][0] != 0) return r[0][0];
	if(r[0][1] == r[1][1] && r[1][1] == r[2][1] && r[0][1] != 0) return r[0][1];
	if(r[0][2] == r[1][2] && r[1][2] == r[2][2] && r[0][2] != 0) return r[0][2];
	//Jossei voittoa löytynyt, tarkista onko ruudukossa tyhjiä ruutuja jäljellä.
	//Kaksi sisäkkäistä for silmukkaa käy läpi koko ruudukon:
	for(int i = 0; i < 3; i++)
	{
		for(int i2 = 0; i2 < 3; i2++)
		{
			if(r[i][i2] == 0) return 0;
		}
	}
	//Jos tyhjiä ruutuja ei ole, ja kumpikaan pelaaja ei ole voittanut peliä, on tilanne tasapeli.
	return 3;
}
int main()
{	
	//Tee ikkuna johon voimme piirtää:
	RenderWindow window(VideoMode(600,600), "Ristinollapeli");
	//Ruudukko yksinkertaisessa muodossa, eli INT taulukossa, jota voimme helpommin käyttää funktioissa, kuten voitontarkistuksessa
	int simple_ruudut[3][3];
	//Ruudukon suorakulmioiden taulukko, käytetään graafisessa esityksessä.
	RectangleShape ruudut[3][3];
	/*  For silmukka täyttää ruudukon valkoisilla ruuduilla.
	 *  Samalla asettaa ruuduille mustat reunukset, jotka eivät muutu ruudun väriä vaihdettaessa.
	 *  Ruutujen paikat määräytyvät i:n ja i2:n mukaan. Jos kyseessä on ruutu x: 1, y: 2. Ruudun paikaksi tulee x: 200 ja y 400.
	 *  Jos kyseessä on ruutu x: 0, y: 1. Ruudun paikaksi tulee x: 0 ja y: 200
	 *  X ja Y ovat ruudun vasemman yläkulman koordinaatit, laskettuna siten, että ikkunan x: 0 ja y: 0 on ikkunan vasen yläkulma.
	 */
	for(int i = 0; i < 3; i++)
	{
		for(int i2 = 0; i2 < 3; i2++)
		{
			//Ruudun koko ja muoto, tässä tapauksessa 200.
			RectangleShape r(Vector2f(200,200));
			//Ruudun alkuperäinen väri.
			r.setFillColor(Color::White);
			//Ruudun paikka, selitetty yläpuolella.
			r.setPosition(i2*200,i*200);
			//Ruudun reunuksien paksuus.
			r.setOutlineThickness(10);
			//Ruudun reunuksien väri.
			r.setOutlineColor(Color::Black);
			//Aseta äsken tehty ruutu oikealle paikalleen.
			ruudut[i][i2] = r;
			//Aseta yksinkertaisen taulukkomme ruutu tyhjäksi.
			simple_ruudut[i][i2] = 0;
		}
	}
	//Muuttuja joka pitää kirjaa siitä kenen vuoro tällä hetkellä on.
	bool turn = false;
	//Muuttuja joka varmistaa, että klikkaus rekisteröidään vain kerran, eikä jokaisella framella jolla hiiren vasen painike on alhaalla.
	bool already_down = false;
	//Pelin pääsilmukka, joka pyörii niin kauan kuin pelin ikkuna on auki.
	while(window.isOpen())
	{
		//Pieni viive, koska ristinollaa on aivan turha pyörittää yli 9000 fps.
		sleep(milliseconds(10));
		Event event;
		//Silmukka joka käy läpi ikkunaan kohdistuvat tapahtumat.
		while(window.pollEvent(event))
		{
			//Jos tapahtuma on se, että ikkunaa yritetään sulkea (X painike):
			if(event.type == Event::Closed)
			{
				//Sulje ikkuna, ja koska ikkuna sulkeutuu niin pääsilmukka katkeaa.
				window.close();
			}
		}	
		//Täytä ikkuna mustalla, jotteivat edellisen framen aikana piirretyt asiat näy.
		//Tässä tapauksessa tätä ei tarvita välttämättä, koska pelimme piirtää kaikkien asioiden yli joka framella,
		//mutta tämän vaikutus ohjelman tehokkuuteen on niin pieni että tämä voi aivan hyvin olla olemassa.
		window.clear(Color::Black);
		//Jos hiirtä ei ole painettu alas, mutta viime framen aikana oli, muuta muuttuja epätodeksi.
		if(!Mouse::isButtonPressed(Mouse::Left) && already_down) already_down = false;
		//Jos hiiren vasen painike on painettuna alas, ja se ei ollut alhaalla viime framen aikana.
		if(Mouse::isButtonPressed(Mouse::Left) && !already_down)
		{
			//Ota hiiren nykyinen sijainti muuttujaan.
			Vector2i lpos = Mouse::getPosition(window);
			//Jos on pelaaja 1. vuoro:
			if(!turn)
			{
				//Jos klikattu ruutu on tyhjä.
				if(simple_ruudut[lpos.y/200][lpos.x/200] == 0)
				{
					/* Muuta painetun ruudun väri siniseksi.
					 * Muutettava ruutu määritellään jakamalla hiiren y ja x koordinaatit 200:lla jolloin saadaan yksinumeroinen luku
					 * joka kertoo osoitetun ruudun x ja y koordinaatit taulukossamme (0 - 5).
					 */
					ruudut[lpos.y/200][lpos.x/200].setFillColor(Color::Blue);
					//Muuta vuoromuuttuja osoittamaan, että on pelaaja 2. vuoro.
					turn = true;
					//Aseta yksinkertaisesta int ruudukosta ruutu siniseksi.
					simple_ruudut[lpos.y/200][lpos.x/200] = 1;
				}
			}
			//Jos on pelaaja 2. vuoro, ja äsken ei ollut pelaaja 1. vuoro:
			else {
				if(turn)
				{
					//Tarkisa että ruutu on tyhjä.
					if(simple_ruudut[lpos.y/200][lpos.x/200] == 0)
					{
						//Sama selitys kuin pelaaja 1. tapauksessa, mutta väri muutetaan punaiseksi.
						ruudut[lpos.y/200][lpos.x/200].setFillColor(Color::Red);
						//Muuta vuoromuuttuja osoittamaan, että on pelaaja 1. vuoro.
						turn = false;
						//Aseta yksinkertaisesta int ruudukosta ruutu punaiseksi.
						simple_ruudut[lpos.y/200][lpos.x/200] = 2;
					}
				}
			}
			//Muuta muuttujaa niin, että se kertoo hiiren painikkeen olleen jo alhaalla, jottei uutta painallusta
			//rekisteröidä jollei painike käy välissä ylhäällä.
			already_down = true;
		}
		//Käy kahdella sisäkkäisellä for silmukalla ruudukon ruudut läpi, ja piirrä ne.
		for(int i = 0; i < 3; i++)
		{
			for(int i2 = 0; i2 < 3; i2++)
			{
				//Piirrä ruutu joka on paikassa x: i ja y: i2.
				window.draw(ruudut[i][i2]);
			}
		}
		//Näytä ikkunassa kaikki piirtämämme.
		window.display();
		//Jos jokin pelin päättävä tilanne löytyy, sulje ikkuna, ja kerro konsolissa kumpi voitti.
		int voitto_tila = voitto(simple_ruudut);
		if(voitto_tila == 1) {cout << "VOITTO: SININEN" << "\n"; window.close();}
		if(voitto_tila == 2) {cout << "VOITTO: PUNAINEN" << "\n"; window.close();}
		if(voitto_tila == 3) {cout << "TASAPELI" << "\n"; window.close();}
	}
	//Poistu ohjelmasta jos while silmukka meni rikki.
	return 0;
}	
