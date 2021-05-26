/*
  Aadithya Sairam

  This circuit is a basic math game with generates random simple math questions and keeps track of score

  The circuit:
  - Arduino
  - Breadboard
  - Piezo Buzzer
  - Green and Red LED
  - LCD
  - 4x4 Keypad
  - 2 1kohm resistor
  - 5k ohm resistor
  - 220 ohm resistor
  - Jumper cables
  Created: 21/01/2021
  By: Aadithya Sairam
*/

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, A5, A3, 12, A4, 8);

// initialize the keypad
const byte ROWS = 4;
const byte COLS = 4;

char keys [ROWS] [COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'-', '0', '.', 'D'}
};

byte rowPins[ROWS] = {3, 2, 1, 0};
byte colPins[COLS] = {7, 6, 5, 4};
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins,
                          ROWS, COLS );

//Variables
const int buzzer = 11;
const int buzzerDur = 150;
const int buzzerDel = 50;
const int green = 9;
const int red = 10;
boolean pageOne = false;
boolean difSelect = true;
boolean timerEnd = false;
bool timeON = false;
char key;
String oper;
String UserInput;
String change;
float ans;
int randOper;
int rand1;
int rand2;
int changeInt;
int divAdjust;
int cursorPosition = -1;
int score = 0;
int total = 0;
int difChoice;
int time;
int start;
int s, s1, s10;
int current;
int timeSet;


//Function ansCheck(a,b) has the parameters user(String) which is the user input and Comp(float) the computers answer to the question
bool ansCheck(String user, float Comp) {
  change = "";
  //Logic to combat errors with reading negative value
  if (user.indexOf('-') != -1) {
    for (int i = 1; i <= user.length(); i++) {
      change = change + user[i];
    }
    changeInt = 0 - change.toInt();
    if (changeInt == Comp) {
      return true;
    } else {
      return false;
    }
  } else if ((user.toFloat()) == (Comp)) {
    return true;
  } else {
    return false;
  }
}

//numpad() method checks if values on the numpad are clicked and returns TRUE(1) when a valid number is pressed otherwise it stays FALSE(0)
bool numpad() {
  if (key == '1' && pageOne || key == '2' && pageOne ||
      key == '3' && pageOne || key == '4' && pageOne ||
      key == '5' && pageOne || key == '6' && pageOne ||
      key == '7' && pageOne || key == '8' && pageOne ||
      key == '9' && pageOne || key == '0' && pageOne ||
      key == '-' && pageOne || key == '.' && pageOne
      && (UserInput.indexOf('.') == -1) || key == '1' && difSelect ||
      key == '2' && difSelect || key == '3' && difSelect) {
    return true;
  } else {
    return false;
  }
}
//Function keyPress() recieves the user inputs from the numpad and creates a string of the users input
void keyPress() {
  if (key == '1' && difSelect) {
    difChoice = 1;
  } else if (key == '2' && difSelect) {
    difChoice = 2;
  } else if (key == '3' && difSelect) {
    difChoice = 3;
    //'-' becomes a backspace when the user inputs atleast one value
  } else if (key == '-' && (UserInput.length() > 0)) {
    UserInput.remove(UserInput.length() - 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(rand1);
    lcd.print(oper);
    lcd.print(rand2);
    lcd.print("=");
    cursorPosition = 10 - ((String(score)).length());
    lcd.setCursor(cursorPosition, 0);
    lcd.print("Score:");
    lcd.print(score);
    lcd.setCursor(0, 1);
    lcd.print(UserInput);
    cursorPosition = UserInput.length() - 1;
  } else if (UserInput.length() < 10) {
    cursorPosition = cursorPosition + 1;
    lcd.setCursor(cursorPosition, 2);
    lcd.print(key);
    UserInput = UserInput + key;
  }
}
//Function keyA() is used when 'A' on the keypad is pressed
//keyA() runs the code to generate a new random question and answer
void keyA() {
  pageOne = true;
  timerEnd = true;
  difSelect = false;
  //Creates random values for both terms and operator
  randOper = random(0, 4);
  rand1 = random(0, 10);
  rand2 = random(0, 10);
  //reset the LEDs
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
  //Increments questions asked
  total++;
  //Conditional statement depending on the random operator choosen to calculate correct anser for each question
  lcd.clear();
  if (randOper == 0) {
    oper = "+";
    ans = rand1 + rand2;
  } else if (randOper == 1) {
    oper = "-";
    ans = rand1 - rand2;
  } else if (randOper == 2) {
    oper = "*";
    ans = rand1 * rand2;
  } else if (randOper == 3) {
    //Additional condition for division to prevent irrational, undefined etc. numbers.
    if (rand2 == 3 && rand1 != rand2 && rand1 != 0 && ((rand1 % rand2) != 0) ||
        rand2 == 6 && rand1 != rand2 && rand1 != 0 && ((rand1 % rand2) != 0) ||
        rand2 == 7 && rand1 != rand2 && rand1 != 0 && ((rand1 % rand2) != 0) ||
        rand2 == 9 && rand1 != rand2 && rand1 != 0 && ((rand1 % rand2) != 0) ||
        rand2 == 0) {
      //Values which cause impossible scenarios will change denominator to 1,2,4,5,8 or the same as rand1 to create a possible value for user to input
      divAdjust = random(0, 5);
      if (divAdjust == 0) {
        rand2 = rand1;
      } else if (divAdjust == 1) {
        rand2 = 1;
      } else if (divAdjust == 2) {
        rand2 = 2;
      } else if (divAdjust == 3) {
        rand2 = 4;
      } else if (divAdjust == 4) {
        rand2 = 5;
      } else {
        rand2 = 8;
      }
    }
    oper = "/";
    ans = (float(rand1) / float(rand2));
  }
  //Active score display and setup
  cursorPosition = 10 - ((String(score)).length());
  lcd.setCursor(cursorPosition, 0);
  lcd.print("Score:");
  lcd.print(score);
  //Question display
  cursorPosition = 0;
  lcd.setCursor(cursorPosition, 0);
  lcd.print(rand1);
  lcd.print(oper);
  lcd.print(rand2);
  lcd.print('=');
  cursorPosition = -1;
  UserInput = "";
}
//Function KeyB() is used when 'B' is pressed on the keypad
//KeyB() submits the user answer and displays a corresponding message
void keyB() {
  //Conditional statement determines whether user input is correct or not and takes corresponding actions
  if (ansCheck(UserInput, ans)) {
    //Correct answer message
    lcd.print("Nice! Click A   ");
    //Correct answer tone
    tone(buzzer, 440, buzzerDur);
    delay(buzzerDel);
    tone(buzzer, 550, buzzerDur);
    delay(buzzerDel);
    tone(buzzer, 660, buzzerDur);
    delay(buzzerDel);
    //Green LED turns on and score increments by one
    digitalWrite(green, HIGH);
    score++;
  } else if (!ansCheck(UserInput, ans)) {
    //Incorrect answer message
    lcd.print("Wrong! Click A  ");
    //Incorrect answer tone
    tone(buzzer, 330, buzzerDur);
    delay(buzzerDel);
    tone(buzzer, 220, buzzerDur);
    delay(buzzerDel);
    tone(buzzer, 110, buzzerDur);
    delay(buzzerDel);
    //Red LED turns on
    digitalWrite(red, HIGH);
  }
  timerEnd = false;
  pageOne = false;
}
//Function KeyC() is used when 'C' is pressed on the keypad
//KeyC() changes the display to user's current stats
void keyC() {
  //reset LEDs
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
  //Display total questions asked and total correct answers
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Total:");
  lcd.print(total);
  lcd.setCursor(0, 1);
  lcd.print("Correct:");
  lcd.print(score);
  pageOne = false;
}
//Function KeyD() is used when 'D' is pressed on the keypad
//KeyD() restarts the users stats and opens to starting screen
void keyD() {
  //reset LEDs
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
  //reset variables and display menu screen
  pageOne = false;
  lcd.clear();

  lcd.print("   Welcome to   ");
  lcd.setCursor(0, 1);
  lcd.print(" Epic Math Game ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Difficulty");
  lcd.setCursor(0, 1);
  lcd.print("1-E 2-M 3-H");
  difSelect = true;
  difChoice = 0;

  total = 0;
  score = 0;
}
//Function to perform necessary outputs once time limit is exceeded
void timeDone() {
  lcd.print("Out of Time! 'A'");
  //Incorrect answer tone
  tone(buzzer, 330, buzzerDur);
  delay(buzzerDel);
  tone(buzzer, 220, buzzerDur);
  delay(buzzerDel);
  tone(buzzer, 110, buzzerDur);
  delay(buzzerDel);
  //Red LED turns on
  digitalWrite(red, HIGH);
  timerEnd = false;
  pageOne = false;
}
//Question time limiter
void timer(bool displayTime)
{
  if (displayTime && pageOne && timerEnd) {
    time = (current - (millis() - start));
    //Seconds convert
    s = (time / 1000) % 60;
    s1 = s % 10;
    s10 = (s - s1) / 10;
    lcd.setCursor(9, 1);
    lcd.print("Time:");
    lcd.print(s10);
    lcd.print(s1);
  }
}

//Setup
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //LED and buzzer setup
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // Print a message to the LCD.
  lcd.print("   Welcome to   ");
  lcd.setCursor(0, 1);
  lcd.print(" Epic Math Game ");
  randomSeed(analogRead(0));
  //Setup timer and Difficulty select
  start = millis();
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Difficulty");
  lcd.setCursor(0, 1);
  lcd.print("1-E 2-M 3-H");
}

//Mainloop
void loop() {
  timer(timeON);
  //recieves keypad instructions
  key = myKeypad.getKey();
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  //First recieves user's difficulty choice
  //Checks if a number, decimal or negative is pressed
  //Outputs the corresponding value to the LCD
  if (numpad()) {
    keyPress();
    //Difficulty Selector
    if (difSelect == true) {
      if (difChoice == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Easy Mode");
        lcd.setCursor(0, 1);
        lcd.print("A to Continue");
        timeON = false;
        difChoice = -1;
      } else if (difChoice == 2) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Normal Mode");
        lcd.setCursor(0, 1);
        lcd.print("A to Continue");
        timeON = true;
        timeSet = 20000;
        difChoice = -1;
      } else if (difChoice == 3) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Hard Mode");
        lcd.setCursor(0, 1);
        lcd.print("A to Continue");
        timeON = true;
        timeSet = 10000;
        difChoice = -1;
      }
    }
  }
  //Checks if 'A' is selected
  //Runs the keyA() function for new question
  else if (key == 'A' && !pageOne && difChoice == -1) {
    keyA();
    current = millis() + timeSet;
  }
  //Checks if 'B' is selected
  //Runs the keyB() function for answer submission
  else if (key == 'B' && pageOne && time > 0 && !difSelect && UserInput != ""
           || key == 'B' && pageOne && !timeON && !difSelect && UserInput != "") {
    keyB();
  }
  //Checks if 'C' is selected
  //Runs the keyC() function to display current stats
  else if (key == 'C' && !pageOne && !difSelect) {
    keyC();
  }
  //Checks if 'D' is selected
  //Runs the keyD() function to restart the game
  else if (key == 'D') {
    keyD();
  }
  //Checks timer status and 
  else if (time <= 0 && pageOne && timerEnd && timeON) {
    timeDone();
  }
}