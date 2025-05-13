#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;

int collaborativeFund = 0;

class Player;
class Board;

class Type
{
private:
	string typeName;
	bool isBuyable;
	int typeIndex;
public:
	Type(string name, bool buyable, int index) : typeName(name), isBuyable(buyable), typeIndex(index) {}
	string getTypeName() const
	{
		return typeName;
	}
	bool getIsBuyable() const {
		return isBuyable;
	}

	int getTypeIndex() const
	{
		return typeIndex;
	}

};

class Property : public Type
{
private:
	string propertyName;
	int propertyPrice;
	int propertyRent[6];
	int currentRentLevel;
	int housePrice;
	string color;
	bool isSold;

	void updateTable()
	{
		int i;
		for (i = 0; i < indexTable; i++)
		{
			if (ColorTable[i] == this->color)
			{
				propertyCountTable[i]++;
				break;
			}
		}

		if (i == indexTable)
		{
			const int size = indexTable;
			int* newPropertyCountTable = new int[size + 1];
			string* newColorTable = new string[size + 1];
			for (int i = 0; i < size; i++)
			{
				newColorTable[i] = ColorTable[i];
				newPropertyCountTable[i] = propertyCountTable[i];
			}
			newColorTable[size] = this->color;
			newPropertyCountTable[size] = 1;
			delete[] ColorTable;
			delete[] propertyCountTable;
			ColorTable = newColorTable;
			propertyCountTable = newPropertyCountTable;
			indexTable++;
		}

	}

public:
	static int indexTable;
	static string* ColorTable;
	static int* propertyCountTable;
	
	Property() : Type("Property", true, -1), propertyName(""), propertyPrice(0), currentRentLevel(0), housePrice(0), color(""), isSold(false)
	{
		for (int i = 0; i < 6; i++)
			propertyRent[i] = 0;
	}
	Property(string name, int  index, int price, int rent[], int housePrice, string color)
		: Type("Property", true, index), propertyName(name), propertyPrice(price), currentRentLevel(0), housePrice(housePrice), color(color)
	{
		isSold = false;
		for (int i = 0; i < 6; i++)
			propertyRent[i] = rent[i];
		updateTable();
	}
	string getPropertyName() const
	{
		return propertyName;
	}
	int getPropertyPrice() const
	{
		return propertyPrice;
	}
	int getCurrentRentLevel() const
	{
		return currentRentLevel;
	}
	int getHousePrice() const
	{
		return housePrice;
	}
	string getColor() const
	{
		return color;
	}
	void setCurrentRentLevel(int level)
	{
		if (level >= 0 && level < 6)
			currentRentLevel = level;
	}

	int getPropertyRent() const
	{
		return propertyRent[currentRentLevel];
	}

	bool getIsSold() const
	{
		return isSold;
	}

	void setSold(bool state)
	{
		isSold = state;
	}

	bool canBuildHouse() const
	{
		return currentRentLevel < 5;
	}

	int calculateRent() const
	{
		return propertyRent[currentRentLevel];
	}
};

void setupGameRules()
{
	const int STARTING_MONEY = 1500;
	const int GO_MONEY = 200;
	const int MAX_HOUSES_PER_COLOR = 4;
	const int MAX_HOTELS_PER_COLOR = 1;
}


class Railway : public Type
{
private:
	string railwayName;
	int railwayPrice;
	const int railwayRent[4] = {25,50,100,200};
	bool isSold;
public:

	Railway() : Type("Railway", true, -1), railwayName(""), railwayPrice(0)
	{
		isSold = false;
	}

	Railway(string name, int index, int price)
		: Type("Railway", true, index), railwayName(name), railwayPrice(price)
	{
		isSold = false;
	}

	string getRailwayName() const
	{
		return railwayName;
	}

	int getRailwayPrice() const
	{
		return railwayPrice;
	}

	int getRailwayRent(int count) const
	{
		return railwayRent[count];
	}
	void setSold(bool state) { isSold = state; }
	int calculateRent(int ownedCount) const { return railwayRent[ownedCount - 1]; }
};

class Utility : public Type {
private:
	string utilityName;
	int utilityPrice;
	bool isSold;
	const int rentMultiplier[2] = { 4, 10 };
public:
	Utility(string name, int index, int price) : Type("Utility", true, index), utilityName(name), utilityPrice(price), isSold(false) {}
	string getUtilityName() const { return utilityName; }
	int getUtilityPrice() const { return utilityPrice; }
	bool getIsSold() const { return isSold; }
	void setSold(bool state) { isSold = state; }
	int calculateRent(int diceRoll, int ownedCount) const {
		return diceRoll * rentMultiplier[ownedCount - 1];
	}
};

class Board
{
private:
	sf::Texture	boardTexture;
	sf::Sprite	boardSprite;
	Property* properties;
	Railway* railways;
	Utility* utilities;
public:
	Board()
	{
	utilities = new Utility[2]{
	Utility("Electric Company", 12, 150),
	Utility("Water Works", 28, 150)
		};
		if (!boardTexture.loadFromFile("images/Monopoly Board.png"))
		{
			cout << "Error loading board texture" << endl;
		}
		boardSprite.setTexture(boardTexture);
		int rent[22][6] = {
	{2, 10, 30, 90, 160, 250},    // Property 0
	{4, 20, 60, 180, 320, 450},   // Property 1
	{6, 30, 90, 270, 400, 550},   // Property 2
	{8, 40, 100, 300, 450, 600},  // Property 3
	{10, 50, 150, 450, 625, 750}, // Property 4
	{12, 60, 180, 500, 700, 900}, // Property 5
	{14, 70, 200, 550, 750, 950}, // Property 6
	{16, 80, 220, 600, 800, 1000},// Property 7
	{18, 90, 250, 700, 875, 1050},// Property 8
	{20, 100, 300, 750, 925, 1100},// Property 9
	{22, 110, 330, 800, 975, 1150},// Property 10
	{24, 120, 360, 850, 1025, 1200},// Property 11
	{26, 130, 390, 900, 1100, 1275},// Property 12
	{28, 150, 450, 1000, 1200, 1400},// Property 13
	{30, 170, 500, 1100, 1300, 1500},// Property 14
	{32, 180, 520, 1150, 1350, 1600},// Property 15
	{34, 200, 550, 1200, 1400, 1700},// Property 16
	{36, 220, 600, 1300, 1500, 1800},// Property 17
	{38, 230, 620, 1350, 1600, 1850},// Property 18
	{40, 240, 650, 1400, 1700, 1900},// Property 19
	{42, 250, 700, 1500, 1800, 2000},// Property 20
	{50, 300, 750, 1600, 1900, 2200} // Property 21
		};
		string propertyNames[22] = {
		"Old Stationary Shop",
		"Lab 4",
		"Mico Lab",
		"Old Audi",
		"Front Lawn",
		"Student Center",
		"EE Basement",
		"English Lab 1",
		"Badminton Court",
		"Library",
		"Lab 13",
		"Lobby",
		"Physics Lab",
		"Civil Basement",
		"Table Tennis Area",
		"C-07",
		"Cricket Ground",
		"Open Cafe",
		"Front Terrace",
		"Love Garden",
		"New Audi",
		"New Cafe"
		};

		int propertyPrices[22] = {
		60,   // Old Stationary Shop
		60,   // Lab 4
		100,  // Mico Lab
		100,  // Old Audi
		120,  // Blink Lawn
		140,  // Student Center
		140,  // EE Basement
		160,  // English Lab 1
		180,  // Library
		200,  // Lab 13
		220,  // Lobby
		220,  // Physics Lab
		240,  // Civil Basement
		260,  // C-07
		260,  // Table Tennis Area
		280,  // Cricket Ground
		320,  // Front Terrace
		320,  // Love Garden
		300,  // Open Cafe
		400,  // New Cafe
		350,  // New Audi
		300   // New Cafe
		};

		string colors[22] = {
		"Dark Purple", // Old Stationary Shop
		"Dark Purple", // Lab 4
		"Light Blue",  // Mico Lab
		"Light Blue",  // Old Audi
		"Light Blue",  // Blink Lawn
		"Pink",        // Student Center
		"Pink",        // EE Basement
		"Pink",        // English Lab 1
		"Orange",      // Library
		"Orange",      // Library
		"Orange",      // Lab 13
		"Red",         // Lobby
		"Red",         // Physics Lab
		"Red",         // Civil Basement
		"Yellow",      // C-07
		"Yellow",      // Table Tennis Area
		"Yellow",      // Cricket Ground
		"Green",       // Front Terrace
		"Green",       // Love Garden
		"Green",       // Open Cafe
		"Dark Blue",   // New Audi
		"Dark Blue",   // New Cafe
		};
		int propertyIndexes[22] = {
			1, 3,    
			6, 8, 9, 
			11, 13, 14, 
			16, 18, 19, 
			21, 23, 24, 
			26, 27, 29,
			31, 32, 34, 
			37, 39      
		};


		properties = new Property[22];
		railways = new Railway[4];
		for (int i = 0; i < 22; i++)
		{
			int housePrice;
			if (propertyPrices[i] <= 100)
				housePrice = 50;
			else if (propertyPrices[i] <= 200)
				housePrice = 100;
			else if (propertyPrices[i] <= 300)
				housePrice = 150;
			else
				housePrice = 200;
			properties[i] = Property(propertyNames[i], propertyIndexes[i], propertyPrices[i], rent[i], housePrice, colors[i]);

		}

		railways = new Railway[4]{  
		Railway("Bike Stand", 5, 200),
		Railway("Gate 5", 15, 200),
		Railway("Gate 2", 25, 200),
		Railway("Car Parking", 35, 200)
		};

	}

	void draw(sf::RenderWindow& window)
	{
		boardSprite.setPosition(550, 20);
		window.draw(boardSprite);
	}

	~Board()
	{
		delete[] properties;
		delete[] railways;
		delete[] utilities;
	}
	Property& getPropertyAtPosition(int position)
	{
		for (int i = 0; i < 22; i++)
		{
			if (properties[i].getTypeIndex() == position)
			{
				return properties[i];
			}
		}
		static Property defaultProperty;
		return defaultProperty;
	}
};

class Button {
private:
	sf::RectangleShape button;
	sf::Text buttonText;
	sf::Font font;

public:
	Button(float x, float y, float width, float height, string text, sf::Color idleColor, sf::Color hoverColor)
	{
		if (!font.loadFromFile("LibreBaskerville-Italic.ttf")) {
			cout << "Error loading font" << endl;
		}

		button.setPosition(sf::Vector2f(x, y));
		button.setSize(sf::Vector2f(width, height));
		button.setFillColor(idleColor);

		buttonText.setFont(font);
		buttonText.setString(text);
		buttonText.setCharacterSize(20);
		buttonText.setFillColor(sf::Color::Black);
		sf::FloatRect textBounds = buttonText.getLocalBounds();
		buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
			textBounds.top + textBounds.height / 2.0f);
		buttonText.setPosition(x + width / 2.0f, y + height / 2.0f);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(button);
		window.draw(buttonText);
	}

	bool isMouseOver(sf::RenderWindow& window)
	{
		float mouseX = sf::Mouse::getPosition(window).x;
		float mouseY = sf::Mouse::getPosition(window).y;

		float btnPosX = button.getPosition().x;
		float btnPosY = button.getPosition().y;
		float btnXPosWidth = button.getPosition().x + button.getSize().x;
		float btnYPosHeight = button.getPosition().y + button.getSize().y;

		return (mouseX >= btnPosX && mouseX <= btnXPosWidth &&
			mouseY >= btnPosY && mouseY <= btnYPosHeight);
	}

	void setFillColor(sf::Color color)
	{
		button.setFillColor(color);
	}

	void setText(string text)
	{
		buttonText.setString(text);
	}
};

class Player
{
private:
	string playerName;
	int playerMoney;
	int playerPosition;
	int playerProperties[22];
	int playerRailways[4];
	int playerHouses[22];
	int playerHotels[22];
	int RailwayCount;
	int PropertyCount;
	bool isBankrupt;
	bool inJail;
	int jailTurns;
public:
	
	Player() : playerName(""), playerMoney(0), playerPosition(0), RailwayCount(0), PropertyCount(0), isBankrupt(false), inJail(false), jailTurns(0)
	{
		for (int i = 0; i < 22; i++)
			playerProperties[i] = -1;
		for (int i = 0; i < 4; i++)
			playerRailways[i] = -1;
		for (int i = 0; i < 22; i++)
			playerHouses[i] = 0;
		for (int i = 0; i < 22; i++)
			playerHotels[i] = 0;
	}

	Player(string playerName)
	{
		this->jailTurns = 0;
		this->inJail = false;
		this->isBankrupt = false;
		this->playerName = playerName;
		playerMoney = 1500;
		playerPosition = 0;
		RailwayCount = 0;
		PropertyCount = 0;
		for (int i = 0; i < 22; i++)
			playerProperties[i] = -1;
		for (int i = 0; i < 4; i++)
			playerRailways[i] = -1;
		for (int i = 0; i < 22; i++)
			playerHouses[i] = 0;
		for (int i = 0; i < 22; i++)
			playerHotels[i] = 0;
	}

	void subtract(Property& property)
	{
		for (int i = 0; i < PropertyCount; i++)
		{
			if (playerProperties[i] == property.getTypeIndex())
			{
				playerProperties[i] = -1;
				property.setSold(false);
				break;
			}
		}
	}
	
	void subtract(Railway& railway)
	{
		for (int i = 0; i < RailwayCount; i++)
		{
			if (playerRailways[i] == railway.getTypeIndex())
			{
				playerRailways[i] = -1;
				break;
			}
		}
	}

	void add(Property& property)
	{
		if (PropertyCount < 22)
		{
			playerProperties[PropertyCount] = property.getTypeIndex();
			PropertyCount++;
			property.setSold(true);
		}
	}

	void add(Railway& railway)
	{
		if (RailwayCount < 4)
		{
			playerRailways[RailwayCount] = railway.getTypeIndex();
			RailwayCount++;
		}
	}

	int getPlayerMoney() const
	{
		return playerMoney;
	}

	void Bankrupt()
	{
		isBankrupt = true;
	}

	bool isPlayerBankrupt() const
	{
		return isBankrupt;
	}

	template<typename T>
	void Buy(T& property , Player &seller)
	{
		if (playerMoney >= property.getPropertyPrice())
		{
			playerMoney -= property.getPropertyPrice();
			add(property);
			seller.subtract(property);
		}
		else
		{
			Bankrupt();
		}
	}

	void addMoney(int amount)
	{
		playerMoney += amount;
	}

	void subtractMoney(int amount)
	{
		playerMoney -= amount;
		if (playerMoney < 0)
			Bankrupt();
	}

	void setPlayerPosition(int position)
	{
		playerPosition = position;
	}

	int getPlayerPosition() const
	{
		return playerPosition;
	}

	void transferMoney(Player& player, int amount)
	{
		this->subtractMoney(amount);
		player.addMoney(amount);
	}
	void transferTax(int amount)
	{
		this->subtractMoney(amount);
		collaborativeFund += amount;
	}

	void setInJail(bool state)
	{
		inJail = state;
	}

	bool getInJail() const
	{
		return inJail;
	}

	void getTaxMoney()
	{
		this->addMoney(collaborativeFund);
		collaborativeFund = 0;
	}

	bool buyHouse(Property& property, Board& board)
	{
		if (!ownsAllPropertiesOfColor(property.getColor(), board))
		{
			return false;
		}

		int housePrice = property.getHousePrice();
		if (playerMoney < housePrice)
		{
			return false;
		}

		
		if (property.getCurrentRentLevel() >= 5) 
		{
			return false;
		}

		
		playerMoney -= housePrice;
		property.setCurrentRentLevel(property.getCurrentRentLevel() + 1);

		return true;
	}

	void showHouseBuyDialog(Property& property, Board& board)
	{
		sf::RenderWindow houseWindow(sf::VideoMode(300, 200), "Buy Houses");
		Button buyHouseButton(50, 100, 100, 50, "Buy House", sf::Color::Green, sf::Color::Yellow);
		Button cancelButton(150, 100, 100, 50, "Cancel", sf::Color::Red, sf::Color::Yellow);

		while (houseWindow.isOpen())
		{
			sf::Event event;
			while (houseWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					houseWindow.close();

				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (buyHouseButton.isMouseOver(houseWindow))
					{
						if (buyHouse(property,board))
						{
							houseWindow.close();
						}
						else
						{
							sf::Text errorText;
							errorText.setString("Cannot buy house!");
						}
					}
					else if (cancelButton.isMouseOver(houseWindow))
					{
						houseWindow.close();
					}
				}
			}

			houseWindow.clear(sf::Color::White);

			sf::Text houseInfoText;
			houseInfoText.setString("Current Houses: " +
				to_string(property.getCurrentRentLevel()) +
				"\nHouse Price: $" + to_string(property.getHousePrice()));
			houseWindow.draw(houseInfoText);
			buyHouseButton.draw(houseWindow);
			cancelButton.draw(houseWindow);
			houseWindow.display();
		}
	}
	bool ownsProperty(int propertyIndex)
	{
		for (int i = 0; i < PropertyCount; i++)
		{
			if (playerProperties[i] == propertyIndex)
			{
				return true;
			}
		}
		return false;
	}

	
	bool ownsAllPropertiesOfColor(string color, Board& board)
	{
		int colorCount = 0;
		int ownedColorCount = 0;

		
		for (int i = 0; i < 22; i++)
		{
			Property& prop = board.getPropertyAtPosition(i);
			if (prop.getColor() == color)
			{
				colorCount++;
			}
		}

		for (int i = 0; i < PropertyCount; i++)
		{
			Property& prop = board.getPropertyAtPosition(playerProperties[i]);
			if (prop.getColor() == color)
			{
				ownedColorCount++;
			}
		}

		return colorCount == ownedColorCount;
	}

	string getPlayerName() const
	{
		return playerName;
	}

	void drawPlayer(sf::RenderWindow& window, sf::Font& font, int playerIndex)
	{
		sf::CircleShape playerMarker(10); 
		sf::Color playerColors[4] = {
			sf::Color::Red,
			sf::Color::Blue,
			sf::Color::Yellow,
			sf::Color::Green
		};
		playerMarker.setFillColor(playerColors[playerIndex]);
		playerMarker.setOutlineThickness(2);
		playerMarker.setOutlineColor(sf::Color::Black);


		int boardPositions[40][2] = {
			{1105, 55}, 
			{1105, 105}, 
			{1105, 155},  
			{1105, 205},  
			{1105, 255},  
			{1105, 305},  
			{1105, 355},  
			{1105, 405},  
			{1105, 455},  
			{1105, 505},  
			{1105, 555},  
			
			{1055, 555},  
			{1005, 555},  
			{955, 555},  
			{905, 555},  
			{855, 555},  
			{805, 555},  
			{755, 555},  
			{705, 555},  
			{655, 555},  
			{605, 555},  
			
			{605, 505},
			{605, 455},
			{605, 405},
			{605, 355},   
			{605, 305},   
			{605, 255},   
			{605, 205},   
			{605, 155},   
			{605, 105},   
			{605, 55},
			{655, 55},  
			{705, 55},  
			{755, 55},  
			{805, 55},   
			{855, 55},  
			{905, 55},  
			{955, 55},
			{1005, 55}, 
			{1055, 55},  
		};

		int position = getPlayerPosition();

		playerMarker.setPosition(
			boardPositions[position][0] + (playerIndex * 15) - 30,
			boardPositions[position][1] + (playerIndex * 15) - 20
		);
		window.draw(playerMarker);
		
		sf::Text playerText;
		playerText.setFont(font);
		playerText.setString(playerName.substr(0, 1)); 
		playerText.setCharacterSize(10);
		playerText.setFillColor(sf::Color::Black);
		playerText.setPosition(
			boardPositions[position][0] + (playerIndex * 15) - 25,
			boardPositions[position][1] + (playerIndex * 15) - 15
		);
		window.draw(playerText);
	}
};


int Property::indexTable = 0;
string* Property::ColorTable = nullptr;
int* Property::propertyCountTable = nullptr;


class Chance
{
private:
	string chanceCards[16] = {
		"Advance to Go",
		"Advance to Love Garden",
		"Advance to New Cafe",
		"Advance to Lab 4",
		"Go back 3 spaces",
		"Get out of Jail Free",
		"Bank pays you dividend of 50",
		"Receive 100 from collaborative fund",
		"Pay hospital 100",
		"Pay school tax 150",
		"Collect 50 from each player",
		"Your property is under repair. Pay 25 per house and 100 per hotel",
		"You won a competition. Collect 100",
		"You are assessed for street repairs",
		"You have won second prize in a beauty contest",
		"Income tax refund"
	};

public:
	string drawCard()
	{
		int cardIndex = rand() % 16;
		return chanceCards[cardIndex];
	}
};

class CommunityChest
{
private:
	string communityChestCards[16] = {
		"Advance to Go",
		"Receive 200",
		"Pay hospital 100",
		"Get out of Jail Free",
		"Go to Jail",
		"Collect 50 from each player",
		"Bank error in your favor. Collect 200",
		"Doctor's fee. Pay 50",
		"Receive 100 from insurance",
		"School tax refund. Collect 20",
		"Receive 10 for services",
		"Birthday. Collect 10 from each player",
		"Life insurance matures. Collect 100",
		"Hospital fees. Pay 100",
		"Inheritance. Collect 100",
		"Pay poor tax of 15"
	};

public:
	string drawCard()
	{
		int cardIndex = rand() % 16;
		return communityChestCards[cardIndex];
	}
};

class Dice
{
private:
	int lastRoll;
	bool isDoubles;

public:
	Dice() : lastRoll(0), isDoubles(false) {}

	int roll()
	{
		int die1 = rand() % 6 + 1;
		int die2 = rand() % 6 + 1;

		lastRoll = die1 + die2;
		isDoubles = (die1 == die2);

		return lastRoll;
	}

	int getLastRoll() const
	{
		return lastRoll;
	}

	bool isDoublesRolled() const
	{
		return isDoubles;
	}
};

class TradeOffer
{
private:
	Player* sender;
	Player* receiver;
	vector<Property*> senderProperties;
	vector<Property*> receiverProperties;
	int senderMoney;
	int receiverMoney;

public:
	TradeOffer(Player* from, Player* to)
	{
		sender = from;
		receiver = to;
		senderMoney = 0;
		receiverMoney = 0;
	}

	void addSenderProperty(Property* prop)
	{
		senderProperties.push_back(prop);
	}

	void addReceiverProperty(Property* prop)
	{
		receiverProperties.push_back(prop);
	}

	void setSenderMoney(int amount)
	{
		senderMoney = amount;
	}

	void setReceiverMoney(int amount)
	{
		receiverMoney = amount;
	}

	bool executeTrade()
	{
		if (sender->getPlayerMoney() < senderMoney ||
			receiver->getPlayerMoney() < receiverMoney)
		{
			return false;
		}

		for (Property* prop : senderProperties)
		{
			sender->subtract(*prop);
			receiver->add(*prop);
		}

		for (Property* prop : receiverProperties)
		{
			receiver->subtract(*prop);
			sender->add(*prop);
		}

		sender->subtractMoney(senderMoney);
		sender->addMoney(receiverMoney);
		receiver->subtractMoney(receiverMoney);
		receiver->addMoney(senderMoney);

		return true;
	}
	
	void showTradeDialog(Board & board)
	{
		sf::RenderWindow tradeWindow(sf::VideoMode(800, 600), "Trade Offer");
		sf::Font font;
		if (!font.loadFromFile("LibreBaskerville-Italic.ttf")) {
			cout << "Error loading font" << endl;
		}

		sf::Text senderTitle, receiverTitle, moneyTitle;
		senderTitle.setFont(font);
		senderTitle.setString(sender->getPlayerName() + " offers:");
		senderTitle.setCharacterSize(18);
		senderTitle.setFillColor(sf::Color::Black);
		senderTitle.setPosition(50, 20);

		receiverTitle.setFont(font);
		receiverTitle.setString(receiver->getPlayerName() + " offers:");
		receiverTitle.setCharacterSize(18);
		receiverTitle.setFillColor(sf::Color::Black);
		receiverTitle.setPosition(400, 20);

		moneyTitle.setFont(font);
		moneyTitle.setString("Money:");
		moneyTitle.setCharacterSize(18);
		moneyTitle.setFillColor(sf::Color::Black);
		moneyTitle.setPosition(50, 350);

		vector<Button*> senderPropButtons;
		vector<Button*> receiverPropButtons;
		vector<bool> senderPropSelected(22, false);
		vector<bool> receiverPropSelected(22, false);

		int senderPropCount = 0;
		for (int i = 0; i < 22; i++) {
			if (sender->ownsProperty(i)) {
				Button* btn = new Button(50, 60 + senderPropCount * 40, 300, 30,
					"Property " + to_string(i), sf::Color::Cyan, sf::Color::Yellow);
				senderPropButtons.push_back(btn);
				senderPropCount++;
			}
		}

		int receiverPropCount = 0;
		for (int i = 0; i < 22; i++) {
			if (receiver->ownsProperty(i)) {
				Button* btn = new Button(400, 60 + receiverPropCount * 40, 300, 30,
					"Property " + to_string(i), sf::Color::Cyan, sf::Color::Yellow);
				receiverPropButtons.push_back(btn);
				receiverPropCount++;
			}
		}

		string senderMoneyStr = "0";
		string receiverMoneyStr = "0";
		bool senderMoneyActive = false;
		bool receiverMoneyActive = false;

		Button confirmButton(250, 500, 100, 50, "Confirm", sf::Color::Green, sf::Color::Yellow);
		Button cancelButton(450, 500, 100, 50, "Cancel", sf::Color::Red, sf::Color::Yellow);

		while (tradeWindow.isOpen()) {
			sf::Event event;
			while (tradeWindow.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					tradeWindow.close();

				if (event.type == sf::Event::TextEntered) {
					if (senderMoneyActive && event.text.unicode >= '0' && event.text.unicode <= '9') {
						senderMoneyStr += event.text.unicode;
					}
					else if (senderMoneyActive && event.text.unicode == '\b' && !senderMoneyStr.empty()) {
						senderMoneyStr.pop_back();
					}
					if (receiverMoneyActive && event.text.unicode >= '0' && event.text.unicode <= '9') {
						receiverMoneyStr += event.text.unicode;
					}
					else if (receiverMoneyActive && event.text.unicode == '\b' && !receiverMoneyStr.empty()) {
						receiverMoneyStr.pop_back();
					}
				}

				if (event.type == sf::Event::MouseButtonPressed) {

					for (size_t i = 0; i < senderPropButtons.size(); i++) {
						if (senderPropButtons[i]->isMouseOver(tradeWindow)) {
							senderPropSelected[i] = !senderPropSelected[i];
							senderPropButtons[i]->setFillColor(senderPropSelected[i] ? sf::Color::Magenta : sf::Color::Cyan);
						}
					}
					for (size_t i = 0; i < receiverPropButtons.size(); i++) {
						if (receiverPropButtons[i]->isMouseOver(tradeWindow)) {
							receiverPropSelected[i] = !receiverPropSelected[i];
							receiverPropButtons[i]->setFillColor(receiverPropSelected[i] ? sf::Color::Magenta : sf::Color::Cyan);
						}
					}
					sf::FloatRect senderMoneyRect(50, 380, 100, 30);
					sf::FloatRect receiverMoneyRect(400, 380, 100, 30);
					sf::Vector2i mousePos = sf::Mouse::getPosition(tradeWindow);
					if (senderMoneyRect.contains(mousePos.x, mousePos.y)) {
						senderMoneyActive = true;
						receiverMoneyActive = false;
					}
					else if (receiverMoneyRect.contains(mousePos.x, mousePos.y)) {
						receiverMoneyActive = true;
						senderMoneyActive = false;
					}
					else {
						senderMoneyActive = false;
						receiverMoneyActive = false;
					}
					if (confirmButton.isMouseOver(tradeWindow)) {
						senderProperties.clear();
						receiverProperties.clear();
						int senderPropIndex = 0;
						for (int i = 0; i < 22; i++) {
							if (sender->ownsProperty(i) && senderPropSelected[senderPropIndex]) {
								senderProperties.push_back(&board.getPropertyAtPosition(i));
								senderPropIndex++;
							}
						}
						int receiverPropIndex = 0;
						for (int i = 0; i < 22; i++) {
							if (receiver->ownsProperty(i) && receiverPropSelected[receiverPropIndex]) {
								receiverProperties.push_back(&board.getPropertyAtPosition(i));
								receiverPropIndex++;
							}
						}

						senderMoney = senderMoneyStr.empty() ? 0 : stoi(senderMoneyStr);
						receiverMoney = receiverMoneyStr.empty() ? 0 : stoi(receiverMoneyStr);

						if (executeTrade()) {
							sf::RenderWindow successWindow(sf::VideoMode(300, 150), "Trade Complete");
							sf::Text successText;
							successText.setFont(font);
							successText.setString("Trade completed successfully!");
							successText.setCharacterSize(16);
							successText.setFillColor(sf::Color::Black);
							successText.setPosition(50, 50);

							while (successWindow.isOpen()) {
								sf::Event e;
								while (successWindow.pollEvent(e)) {
									if (e.type == sf::Event::Closed ||
										e.type == sf::Event::MouseButtonPressed)
										successWindow.close();
								}

								successWindow.clear(sf::Color::White);
								successWindow.draw(successText);
								successWindow.display();
							}

							tradeWindow.close();
						}
						else {
							sf::Text errorText;
							errorText.setFont(font);
							errorText.setString("Not enough money for trade!");
							errorText.setCharacterSize(16);
							errorText.setFillColor(sf::Color::Red);
							errorText.setPosition(200, 450);
							tradeWindow.draw(errorText);
						}
					}
					else if (cancelButton.isMouseOver(tradeWindow)) {
						tradeWindow.close();
					}
				}
			}

			tradeWindow.clear(sf::Color::White);

			tradeWindow.draw(senderTitle);
			tradeWindow.draw(receiverTitle);
			tradeWindow.draw(moneyTitle);

			for (auto btn : senderPropButtons) {
				btn->draw(tradeWindow);
			}
			for (auto btn : receiverPropButtons) {
				btn->draw(tradeWindow);
			}

			sf::RectangleShape senderMoneyBox(sf::Vector2f(100, 30));
			senderMoneyBox.setPosition(50, 380);
			senderMoneyBox.setFillColor(senderMoneyActive ? sf::Color::Yellow : sf::Color::White);
			senderMoneyBox.setOutlineColor(sf::Color::Black);
			senderMoneyBox.setOutlineThickness(1);

			sf::RectangleShape receiverMoneyBox(sf::Vector2f(100, 30));
			receiverMoneyBox.setPosition(400, 380);
			receiverMoneyBox.setFillColor(receiverMoneyActive ? sf::Color::Yellow : sf::Color::White);
			receiverMoneyBox.setOutlineColor(sf::Color::Black);
			receiverMoneyBox.setOutlineThickness(1);

			sf::Text senderMoneyText, receiverMoneyText;
			senderMoneyText.setFont(font);
			senderMoneyText.setString("$" + (senderMoneyStr.empty() ? "0" : senderMoneyStr));
			senderMoneyText.setCharacterSize(16);
			senderMoneyText.setFillColor(sf::Color::Black);
			senderMoneyText.setPosition(60, 385);

			receiverMoneyText.setFont(font);
			receiverMoneyText.setString("$" + (receiverMoneyStr.empty() ? "0" : receiverMoneyStr));
			receiverMoneyText.setCharacterSize(16);
			receiverMoneyText.setFillColor(sf::Color::Black);
			receiverMoneyText.setPosition(410, 385);

			tradeWindow.draw(senderMoneyBox);
			tradeWindow.draw(receiverMoneyBox);
			tradeWindow.draw(senderMoneyText);
			tradeWindow.draw(receiverMoneyText);

			confirmButton.draw(tradeWindow);
			cancelButton.draw(tradeWindow);

			tradeWindow.display();
		}

		for (auto btn : senderPropButtons) {
			delete btn;
		}
		for (auto btn : receiverPropButtons) {
			delete btn;
		}
	}
};

class GameLogic
{
private:
	Player* players;
	int numPlayers;
	Board board;
	Dice dice;
	int currentPlayerIndex;
	Button* rollDiceButton;
	Button* endTurnButton;
	sf::Text diceRollText;
	sf::Font font;
	bool hasRolledThisTurn;
	int doubleRollCount;

public:
	GameLogic(int playerCount, string playerNames[])
	{
		numPlayers = playerCount;
		players = new Player[numPlayers];

		for (int i = 0; i < numPlayers; i++)
		{
			players[i] = Player(playerNames[i]);
		}

		currentPlayerIndex = 0;

		if (!font.loadFromFile("LibreBaskerville-Italic.ttf")) {
			cout << "Error loading font" << endl;
		}
		diceRollText.setFont(font);
		diceRollText.setCharacterSize(30);
		diceRollText.setFillColor(sf::Color::Black);
		diceRollText.setPosition(350, 500);

		rollDiceButton = new Button(300, 600, 150, 50, "Roll Dice",
			sf::Color::Green, sf::Color::Yellow);
		endTurnButton = new Button(500, 600, 150, 50, "End Turn",
			sf::Color::Red, sf::Color::Yellow);
		hasRolledThisTurn = false;
		doubleRollCount = 0;
	}

	void handlePropertyLanding(Player& player, int position)
	{
		Property& property = board.getPropertyAtPosition(position);
		if (property.getTypeIndex() == -1) {
			return; 
		}

		if (!property.getIsSold()) {
			sf::RenderWindow buyWindow(sf::VideoMode(300, 200), "Buy Property");
			Button buyButton(50, 100, 100, 50, "Buy", sf::Color::Green, sf::Color::Yellow);
			Button skipButton(150, 100, 100, 50, "Skip", sf::Color::Red, sf::Color::Yellow);

			while (buyWindow.isOpen()) {
				sf::Event event;
				while (buyWindow.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						buyWindow.close();

					if (event.type == sf::Event::MouseButtonPressed) {
						if (buyButton.isMouseOver(buyWindow)) {
							if (player.getPlayerMoney() >= property.getPropertyPrice()) {
								player.subtractMoney(property.getPropertyPrice());
								player.add(property);
								buyWindow.close();
							}
						}
						else if (skipButton.isMouseOver(buyWindow)) {
							buyWindow.close();
						}
					}
				}

				buyWindow.clear(sf::Color::White);
				sf::Text propertyText;
				propertyText.setFont(font);
				propertyText.setCharacterSize(20);
				propertyText.setFillColor(sf::Color::Black);
				propertyText.setString("Property: " + property.getPropertyName() +
					"\nPrice: $" + to_string(property.getPropertyPrice()));
				propertyText.setPosition(10, 50);

				buyWindow.draw(propertyText);
				buyButton.draw(buyWindow);
				skipButton.draw(buyWindow);

				buyWindow.display();
			}
		}
		else {
			for (int i = 0; i < numPlayers; i++) {
				if (i != currentPlayerIndex && players[i].ownsProperty(property.getTypeIndex())) {
					int rent = property.calculateRent();
					if (player.getPlayerMoney() >= rent) {
						player.transferMoney(players[i], rent);
						sf::RenderWindow rentWindow(sf::VideoMode(300, 150), "Rent Payment");
						sf::Text rentText;
						rentText.setFont(font);
						rentText.setString("Paid $" + to_string(rent) + " rent to " + players[i].getPlayerName());
						rentText.setCharacterSize(16);
						rentText.setFillColor(sf::Color::Black);
						rentText.setPosition(50, 50);

						Button okButton(100, 100, 100, 50, "OK", sf::Color::Green, sf::Color::Yellow);

						while (rentWindow.isOpen()) {
							sf::Event event;
							while (rentWindow.pollEvent(event)) {
								if (event.type == sf::Event::Closed ||
									(event.type == sf::Event::MouseButtonPressed && okButton.isMouseOver(rentWindow))) {
									rentWindow.close();
								}
							}

							rentWindow.clear(sf::Color::White);
							rentWindow.draw(rentText);
							okButton.draw(rentWindow);
							rentWindow.display();
						}
					}
					else {
						player.Bankrupt();
					}
					break;
				}
			}
		}
	}


	void movePlayer()
	{
		Player& currentPlayer = players[currentPlayerIndex];

		if (currentPlayer.isPlayerBankrupt()) {
			nextTurn();
			return;
		}

		int rollResult = dice.roll();

		diceRollText.setString("Dice Roll: " + to_string(rollResult));

		int currentPosition = currentPlayer.getPlayerPosition();
		int newPosition = (currentPosition + rollResult) % 40;

		if (newPosition < currentPosition) {
			currentPlayer.addMoney(200); 
		}

		currentPlayer.setPlayerPosition(newPosition);
		if (newPosition == 0) {
			currentPlayer.addMoney(200);
		}
		else if (newPosition == 4) {
			currentPlayer.transferTax(200);
		}
		else if (newPosition == 38) {
			currentPlayer.transferTax(100);
		}
		else if (newPosition == 30) {
			currentPlayer.setPlayerPosition(10);
			currentPlayer.setInJail(true);
		}
		else if (newPosition == 20) {
			currentPlayer.getTaxMoney();
		}
		else {

			Property& property = board.getPropertyAtPosition(newPosition);

			if (property.getTypeIndex() != -1) {
				handlePropertyLanding(currentPlayer, newPosition);
			}
			else {
				handleSpecialSquare(currentPlayer, newPosition);
			}
		}
		hasRolledThisTurn = true;
		if (dice.isDoublesRolled()) {
			doubleRollCount++;
			if (doubleRollCount >= 3) {
				currentPlayer.setInJail(true);
				currentPlayer.setPlayerPosition(10);
				nextTurn();
			}
			else {
				hasRolledThisTurn = false;
			}
		}
		else {
			doubleRollCount = 0;
		}
	}

	void drawGameControls(sf::RenderWindow& window)
	{
		window.draw(diceRollText);
		rollDiceButton->draw(window);
		endTurnButton->draw(window);
	}

	bool handleGameControls(sf::RenderWindow& window, sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (rollDiceButton->isMouseOver(window))
			{
				if (!hasRolledThisTurn)
				{
					movePlayer();
					return true;
				}
			}
			else if (endTurnButton->isMouseOver(window))
			{
				hasRolledThisTurn = false;
				doubleRollCount = 0;
				nextTurn();
				return true;
			}
			else if (endTurnButton->isMouseOver(window))
			{
				if (hasRolledThisTurn) {
					hasRolledThisTurn = false;
					doubleRollCount = 0;
					nextTurn();
					return true;
				}
				else {
					diceRollText.setString("Roll dice first!");
					diceRollText.setFillColor(sf::Color::Red);
					return false;
				}
			}
		}
		return false;
	}

	void nextTurn()
	{
		currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
	}

	Player& getCurrentPlayer()
	{
		return players[currentPlayerIndex];
	}

	Player& getPlayerByIndex(int index)
	{
		return players[index];
	}

	Property& getPropertyByIndex(int index)
	{
		return board.getPropertyAtPosition(index);
	}

	~GameLogic()
	{
		delete[] players;
		delete rollDiceButton;
		delete endTurnButton;
	}

	void handleSpecialSquare(Player& player, int position) {

		if (position == 2 || position == 17 || position == 33) {
			CommunityChest communityChest;
			string cardDrawn = communityChest.drawCard();

			sf::RenderWindow cardWindow(sf::VideoMode(400, 300), "Qudrat (Community Chest)");
			sf::Font font;
			if (!font.loadFromFile("LibreBaskerville-Italic.ttf")) {
				cout << "Error loading font" << endl;
			}

			sf::Text cardText;
			cardText.setFont(font);
			cardText.setString(cardDrawn);
			cardText.setCharacterSize(24);
			cardText.setFillColor(sf::Color::Black);
			cardText.setPosition(20, 80);

			Button okButton(150, 200, 100, 50, "OK", sf::Color::Green, sf::Color::Yellow);

			while (cardWindow.isOpen()) {
				sf::Event event;
				while (cardWindow.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						cardWindow.close();
					}

					if (event.type == sf::Event::MouseButtonPressed) {
						if (okButton.isMouseOver(cardWindow)) {
							cardWindow.close();
						}
					}
				}

				cardWindow.clear(sf::Color(230, 230, 180));
				cardWindow.draw(cardText);
				okButton.draw(cardWindow);
				cardWindow.display();
			}
			processCommunityChestCard(player, cardDrawn);
		}
		if (position == 7 || position == 22 || position == 36) {
			Chance chance;
			string cardDrawn = chance.drawCard();
			sf::RenderWindow cardWindow(sf::VideoMode(400, 300), "Naseeb (Chance)");
			sf::Font font;
			if (!font.loadFromFile("LibreBaskerville-Italic.ttf")) {
				cout << "Error loading font" << endl;
			}

			sf::Text cardText;
			cardText.setFont(font);
			cardText.setString(cardDrawn);
			cardText.setCharacterSize(24);
			cardText.setFillColor(sf::Color::Black);
			cardText.setPosition(20, 80);

			Button okButton(150, 200, 100, 50, "OK", sf::Color::Green, sf::Color::Yellow);

			while (cardWindow.isOpen()) {
				sf::Event event;
				while (cardWindow.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						cardWindow.close();
					}

					if (event.type == sf::Event::MouseButtonPressed) {
						if (okButton.isMouseOver(cardWindow)) {
							cardWindow.close();
						}
					}
				}

				cardWindow.clear(sf::Color(180, 230, 180));
				cardWindow.draw(cardText);
				okButton.draw(cardWindow);
				cardWindow.display();
			}
			processChanceCard(player, cardDrawn);
		}
	}

	void processCommunityChestCard(Player& player, const string& card) {
		if (card == "Advance to Go") {
			player.setPlayerPosition(0);
			player.addMoney(200);
		}
		else if (card == "Receive 200") {
			player.addMoney(200);
		}
		else if (card == "Pay hospital 100") {
			player.subtractMoney(100);
		}
		else if (card == "Go to Jail") {
			player.setPlayerPosition(10);
			player.setInJail(true);
		}
		else if (card == "Collect 50 from each player") {
			for (int i = 0; i < numPlayers; i++) {
				if (i != currentPlayerIndex) {
					players[i].transferMoney(player, 50);
				}
			}
		}
		else if (card == "Bank error in your favor. Collect 200") {
			player.addMoney(200);
		}
		else if (card == "Doctor's fee. Pay 50") {
			player.subtractMoney(50);
		}
		else if (card == "Receive 100 from insurance") {
			player.addMoney(100);
		}
		else if (card == "School tax refund. Collect 20") {
			player.addMoney(20);
		}
		else if (card == "Receive 10 for services") {
			player.addMoney(10);
		}
		else if (card == "Birthday. Collect 10 from each player") {
			for (int i = 0; i < numPlayers; i++) {
				if (i != currentPlayerIndex) {
					players[i].transferMoney(player, 10);
				}
			}
		}
		else if (card == "Life insurance matures. Collect 100") {
			player.addMoney(100);
		}
		else if (card == "Hospital fees. Pay 100") {
			player.subtractMoney(100);
		}
		else if (card == "Inheritance. Collect 100") {
			player.addMoney(100);
		}
		else if (card == "Pay poor tax of 15") {
			player.subtractMoney(15);
		}
	}

	void processChanceCard(Player& player, const string& card) {
		if (card == "Advance to Go") {
			player.setPlayerPosition(0);
			player.addMoney(200); 
		}
		else if (card == "Advance to Love Garden") {
			int newPosition = 34; 
			player.setPlayerPosition(newPosition);
		}
		else if (card == "Advance to New Cafe") {
			int newPosition = 39; 
			player.setPlayerPosition(newPosition);
		}
		else if (card == "Advance to Lab 4") {
			int newPosition = 3; 
			player.setPlayerPosition(newPosition);
		}
		else if (card == "Go back 3 spaces") {
			int newPosition = (player.getPlayerPosition() - 3) % 40;
			if (newPosition < 0) newPosition += 40;
			player.setPlayerPosition(newPosition);
		}
		else if (card == "Bank pays you dividend of 50") {
			player.addMoney(50);
		}
		else if (card == "Receive 100 from collaborative fund") {
			player.addMoney(100);
			collaborativeFund -= 100;
			if (collaborativeFund < 0) collaborativeFund = 0;
		}
		else if (card == "Pay hospital 100") {
			player.transferTax(100);
		}
		else if (card == "Pay school tax 150") {
			player.transferTax(150);
		}
		else if (card == "Collect 50 from each player") {
			for (int i = 0; i < numPlayers; i++) {
				if (i != currentPlayerIndex) {
					players[i].transferMoney(player, 50);
				}
			}
		}
		else if (card == "Your property is under repair. Pay 25 per house and 100 per hotel") {
			int totalFee = 0;
			player.subtractMoney(totalFee);
		}
		else if (card == "You won a competition. Collect 100") {
			player.addMoney(100);
		}
		else if (card == "You are assessed for street repairs") {
		}
		else if (card == "You have won second prize in a beauty contest") {
			player.addMoney(50);
		}
		else if (card == "Income tax refund") {
			player.addMoney(20);
		}
	}
	void showTradeWindow(int playerIndex) {
		sf::RenderWindow tradeWindow(sf::VideoMode(600, 400), "Trade Offer");
		vector<Button*> playerButtons;
		for (int i = 0; i < numPlayers; i++) {
			if (i != currentPlayerIndex && !players[i].isPlayerBankrupt()) {
				Button* btn = new Button(50, 50 + i * 60, 200, 50,
					"Trade with " + players[i].getPlayerName(),
					sf::Color::Blue, sf::Color::Yellow);
				playerButtons.push_back(btn);
			}
		}

		while (tradeWindow.isOpen()) {
			sf::Event event;
			while (tradeWindow.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					tradeWindow.close();

				if (event.type == sf::Event::MouseButtonPressed) {
					for (size_t i = 0; i < playerButtons.size(); i++) {
						if (playerButtons[i]->isMouseOver(tradeWindow)) {
							int partnerIndex = -1;
							int count = 0;
							for (int j = 0; j < numPlayers; j++) {
								if (j != currentPlayerIndex && !players[j].isPlayerBankrupt()) {
									if (count == i) {
										partnerIndex = j;
										break;
									}
									count++;
								}
							}

							if (partnerIndex != -1) {
								TradeOffer offer(&players[currentPlayerIndex], &players[partnerIndex]);
								offer.showTradeDialog(board);
							}

							tradeWindow.close();
							break;
						}
					}
				}
			}

			tradeWindow.clear(sf::Color::White);

			sf::Text titleText;
			titleText.setFont(font);
			titleText.setString("Select Player to Trade With:");
			titleText.setCharacterSize(24);
			titleText.setFillColor(sf::Color::Black);
			titleText.setPosition(50, 20);
			tradeWindow.draw(titleText);

			for (auto btn : playerButtons) {
				btn->draw(tradeWindow);
			}

			tradeWindow.display();
		}

		for (auto btn : playerButtons) {
			delete btn;
		}
	}
};

class PropertiesWindow {
private:
    sf::RenderWindow propertiesWindow;
    sf::Font font;
    sf::Text propertyTexts[22];
    sf::Text titleText;
    int propertyCount;

    void setupText(sf::Text& text)
    {
        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::Black);
    }

public:
    PropertiesWindow(GameLogic& gameLogic)
    {
        propertiesWindow.create(sf::VideoMode(500, 600), "Player Properties");
        propertiesWindow.setPosition(sf::Vector2i(1300, 100));

        if (!font.loadFromFile("LibreBaskerville-Italic.ttf")) {
            cout << "Error loading font" << endl;
        }

        titleText.setFont(font);
        titleText.setCharacterSize(24);
        titleText.setFillColor(sf::Color::White);
        titleText.setString("Player Properties");
        titleText.setPosition(50, 10);

        for (int i = 0; i < 22; i++) {
            setupText(propertyTexts[i]);
            propertyTexts[i].setPosition(50, 50 + i * 25);
        }
    }

	void update(GameLogic& gameLogic, int currentPlayerIndex)
	{
		propertyCount = 0;
		for (int i = 0; i < 22; i++) {
			propertyTexts[i].setString("");
		}
		Player& currentPlayer = gameLogic.getPlayerByIndex(currentPlayerIndex);
		for (int i = 0; i < 22; i++) {
			if (currentPlayer.ownsProperty(i)) {
				Property& prop = gameLogic.getPropertyByIndex(i);
				if (prop.getTypeIndex() != -1) { 

					char propInfo[256];
					snprintf(propInfo, sizeof(propInfo),
						"%s - %s - $%d - Houses: %d",
						prop.getPropertyName().c_str(),
						prop.getColor().c_str(),
						prop.getPropertyPrice(),
						prop.getCurrentRentLevel()
					);

					propertyTexts[propertyCount].setString(propInfo);
					propertyCount++;
				}
			}
		}
	}

    void show()
    {
        propertiesWindow.clear(sf::Color(200, 200, 255));
        propertiesWindow.draw(titleText);
        for (int i = 0; i < propertyCount; i++) {
            propertiesWindow.draw(propertyTexts[i]);
        }

        propertiesWindow.display();
    }

    bool isOpen() const
    {
        return propertiesWindow.isOpen();
    }

    sf::RenderWindow& getWindow()
    {
        return propertiesWindow;
    }
};

class PlayerInfoPanel {
private:
	sf::RectangleShape panel;
	sf::Font font;
	sf::Text playerTexts[4];
	Button* propertiesButton;
	Button* tradeOfferButton;

public:
	PlayerInfoPanel(GameLogic& gameLogic)
	{
		if (!font.loadFromFile("LibreBaskerville-Italic.ttf")) {
			cout << "Error loading font" << endl;
		}

		panel.setSize(sf::Vector2f(250, 650));
		panel.setPosition(0, 0);
		panel.setFillColor(sf::Color(200, 200, 200));

		for (int i = 0; i < 4; i++) {
			playerTexts[i].setFont(font);
			playerTexts[i].setCharacterSize(20);
			playerTexts[i].setFillColor(sf::Color::Black);
			playerTexts[i].setPosition(10, 50 + i * 100);
		}

		propertiesButton = new Button(50, 500, 150, 50, "Show Properties",
			sf::Color::Green, sf::Color::Yellow);
		tradeOfferButton = new Button(50, 560, 150, 50, "Trade Offer",
			sf::Color::Blue, sf::Color::Yellow);
	}

	void update(GameLogic& gameLogic)
	{
		for (int i = 0; i < 4; i++) {
			Player& player = gameLogic.getPlayerByIndex(i);
			string playerInfo = player.getPlayerName() + "\n" +
				"Money: $" + to_string(player.getPlayerMoney()) + "\n" +
				"Position: " + to_string(player.getPlayerPosition());
			playerTexts[i].setString(playerInfo);
		}
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(panel);

		for (int i = 0; i < 4; i++) {
			window.draw(playerTexts[i]);
		}

		propertiesButton->draw(window);
		tradeOfferButton->draw(window);
	}
	Button* getTradeOfferButton()
	{
		return tradeOfferButton;
	}
	Button* getPropertiesButton()
	{
		return propertiesButton;
	}

	~PlayerInfoPanel()
	{
		delete propertiesButton;
		delete tradeOfferButton;
	}
};

void Game()
{
	string playerNames[4] = { "Player 1", "Player 2", "Player 3", "Player 4" };

	sf::RenderWindow window(sf::VideoMode(1200, 650), "Monopoly Game");
	window.setPosition(sf::Vector2i(100, 100));

	Board board;
	GameLogic gameLogic(4, playerNames);
	PlayerInfoPanel playerInfoPanel(gameLogic);
	PropertiesWindow propertiesWindow(gameLogic);

	int currentPlayerIndex = 0;
	bool propertiesWindowVisible = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (gameLogic.handleGameControls(window, event))
			{
				currentPlayerIndex = (currentPlayerIndex + 1) % 4;
				continue;
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (playerInfoPanel.getPropertiesButton()->isMouseOver(window))
				{
					if (!propertiesWindowVisible)
					{
						propertiesWindow.update(gameLogic, currentPlayerIndex);
						propertiesWindow.show();
						propertiesWindowVisible = true;
					}
					else
					{
						propertiesWindow.getWindow().close();
						propertiesWindowVisible = false;
					}
				}

				if (playerInfoPanel.getTradeOfferButton()->isMouseOver(window)) {
					gameLogic.showTradeWindow(currentPlayerIndex);
				}
			}

			if (propertiesWindow.isOpen())
			{
				sf::Event propEvent;
				while (propertiesWindow.getWindow().pollEvent(propEvent))
				{
					if (propEvent.type == sf::Event::Closed)
					{
						propertiesWindow.getWindow().close();
						propertiesWindowVisible = false;
					}
				}
			}

		}

		window.clear(sf::Color(86, 185, 82));

		board.draw(window);

		playerInfoPanel.update(gameLogic);
		playerInfoPanel.draw(window);

		sf::Font playerFont;
		if (!playerFont.loadFromFile("LibreBaskerville-Italic.ttf")) {
			cout << "Error loading font for players" << endl;
		}
		gameLogic.drawGameControls(window);
		for (int i = 0; i < 4; i++) {
			Player& player = gameLogic.getPlayerByIndex(i);
			if (!player.isPlayerBankrupt()) {
				player.drawPlayer(window, playerFont, i);
			}
		}
		window.display();
	}
}

int main()
{
	srand(time(NULL));

	if (sf::Shader::isAvailable())
	{
		cout << "Shaders are supported" << endl;
	}

	Property::ColorTable = new string[10];
	Property::propertyCountTable = new int[10]; 
	Game();
	delete[] Property::ColorTable;
	delete[] Property::propertyCountTable;

	return 0;
}