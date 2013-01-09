#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

using std::cin;			using std::cout;		
using std::endl;		using std::stringstream;
using std::string;

string checkAnswer(int guess, int num);

int main()
{	
	//Pick a random number
	srand(time(NULL));
	const int num = rand() % 100 + 1 ;

	//Display instruction to the user
	cout<< "welcome to guess-that-numbers-game!"
		<<" I have already picked a number in [1,100]"<<endl
		<<"Please make a guess. Type \"Exit\" to quit";

	//Declare variables we need
	string answer, guess;
	int convertedGuess;

	//Loop until answer is correct or user types "Exit"
	 while (answer != "Correct! That is my number, you win!")
	 {
		 //Read input
		cin >> guess;

		//If a number check the answer
		if (stringstream(guess) >> convertedGuess)		
			answer = checkAnswer(convertedGuess, num);
		else
			//input is a string, if it is "Exit" then stop loop
			//else ask for a valid value
			if(guess == "Exit")
				break;
			else
				answer = "Please enter a valid value";
		//Print the answer
		 cout << answer << endl;
	 }

	 //Thank user and hold the screen
	 cout<<"\nThanks for playing! press enter to close...";
	 cin.get();	 cin.get();
}

string checkAnswer(int guess, int num)
{
	//Check if the number is valid and return an answer
	if (guess == num)
		return "Correct! That is my number, you win!";
	else if (guess < num)
		return "To low, Guess again: ";
	else if (guess > num && guess < 101)
		return "Too high!, guess again: ";
	else if (guess < 1 || guess > 100)
		return "Please enter a number between 1 and 100";
}