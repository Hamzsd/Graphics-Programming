//#include <iostream>
//#include <string>
//#include <iomanip>
//#include <ios>
//#include <vector>
//#include <algorithm>
//#include <sstream>
//
//using std::cin;			using std::setprecision;
//using std::cout;		using std::string;
//using std::endl;		using std::streamsize;
//using std::vector;      using std::stringstream;
//
//
//string checkAnswer(int guess, int num);
//
//int main()
//{	
//	//Pick a random number
//	srand(time(NULL));
//	const int num = rand() % 100 + 1 ;
//
//	//Display instruction to the user
//	cout<< "welcome to guess-that-numbers-game!"
//		<<" I have already picked a number in [1,100]"<<endl
//		<<"Please make a guess. Type \"Exit\" to quit";
//
//	//Declare variables we need
//	string answer, guess;
//	int convertedGuess;
//
//	//Loop until answer is correct or user types "Exit"
//	 while (answer != "Correct! That is my number, you win!")
//	 {
//		 //Read input
//		cin >> guess;
//
//		//If a number check the answer
//		if (stringstream(guess) >> convertedGuess)		
//			answer = checkAnswer(convertedGuess, num);
//		else
//			//input is a string, if it is "Exit" then stop loop
//			//else ask for a valid value
//			if(guess == "Exit")
//				break;
//			else
//				answer = "Please enter a valid value";
//		//Print the answer
//		 cout << answer << endl;
//	 }
//
//	 //Thank user and hold the screen
//	 cout<<"\nThanks for playing! press enter to close...";
//	 cin.get();	 cin.get();
//}
//
//string checkAnswer(int guess, int num)
//{
//	//Check if the number is valid and return an answer
//	if (guess == num)
//		return "Correct! That is my number, you win!";
//	else if (guess < num)
//		return "To low, Guess again: ";
//	else if (guess > num && guess < 101)
//		return "Too high!, guess again: ";
//	else if (guess < 1 || guess > 100)
//		return "Please enter a number between 1 and 100";
//}


///=======================================================================
// Permute a string example
//void permuteString(string prefix, string str)
//{
//	if ( str == "")
//		cout << prefix << endl;
//	else 
//		for (int i = 0; i < str.length(); i++)
//		{
//			if (str.find(str[i]) == i)
//			{
//				string newPrefix = prefix + str[i];
//				string newStr = str.substr(0, i) + str.substr(i + 1);
//				permuteString(newPrefix, newStr);
//			}
//		}
//}
//
//int main()
//{
//
//	cout<<"Enter a string";
//	string s; 
//	cin >> s;
//
//	permuteString("", s);
//
//	system("pause");
//	
//}
//===========================================================================

////Fizzbuzz program
//int main()
//{
//	//cout << "enter a number: "; 
//	//int x;
//	//cin >> x;
//
//	/*if ( x % 3 == 0)
//		cout<<"fizz";
//	if ( x % 5 == 0)
//		cout << "buzz"<<endl;
//	else if ( x % 3 != 0 && x % 5 != 0)
//		cout<<"Number not divisble by 3 or 5"<<endl;*/
//
//	for (int x = 0; x <= 100; ++x)
//	{
//		if (x % 3 == 0 && x % 5 == 0)
//			cout<<"fizzbuzz"<<endl;
//		else if (x % 5 == 0)
//			cout<<"buzz"<<endl;
//		else if (x % 3 == 0)
//			cout<<"fizz"<<endl;
//		else
//			cout<<x<<endl;
//	}
//
//	system("pause");
//}
//int main()
//{
//	cout << "Please enter your first name: ";
//	string name;
//	cin >> name;
//	cout << "Hello, " << name << "!" << endl;
//
//	cout << "Please enter your midterm and final exam grades: ";
//	double midterm, final;
//	cin >> midterm >> final;
//
//	cout << "Enter all your homework grades, followed by end-of-file: ";
//
//	int count = 0;
//	double sum = 0;
//
//	double x;
//	vector<double> homework;
//
//	while(cin >> x)
//	{
//		++count;
//		sum += x;
//		homework.push_back(x);
//	}
//
//	streamsize prec = cout.precision();
//	cout << "Your final grade is " << setprecision(3)
//		 << 0.2 * midterm + 0.4 * final + 0.4 * sum / count
//		 << setprecision(prec) << endl;
//	
//	string hold;
//	cin>>hold;
//
//	system("pause");
//	
//	return 0;
//}

//int main()
//{
//	std::cout << " Please enter your first name: ";
//	std::string name;
//	std::cin>>name;
//
//	//build the message that we intend to write
//	const std::string greeting = "Hello, " + name + "!";
//
//	//build fourth and second lines
//	const std::string spaces (greeting.size(), ' ');
//	const std::string second = "* " + spaces + " *";
//
//	//build first and fifth lines
//	const std::string first (second.size(), '*');
//
//	//Write it all
//	std::cout<<std::endl;
//	std::cout << first <<std::endl;
//	std::cout << second << std::endl;
//	std::cout << "* " << greeting << " *" << std::endl;
//	std::cout << second << std::endl;
//	std::cout << first <<std::endl;
//
//	std::string hold;
//	std::cin>>hold;
//}