#include "math.h"

int INSTRUCTION_DELAY = 1000;
int BUTTON_DURATION = 200;

int but0, but1, but2, but3, but4, but5, but6, but7,
but8, but9, butTime, butPower, butStop, butStart;

// This routine runs only once upon reset
void setup()
{
    //register spark functions for API
    Spark.function("runInstr", sendInstr);
    Spark.function("mapTest", mappingTest);
    Spark.function("instrTest", encodedInstrTest);
   
    //assign each button to an output
    but0 = D0;
    but1 = D1;
    but2 = D2;
    but3 = D3;
    but4 = D4;
    but5 = D5;
    but6 = D6;
    but7 = D7;
    but8 = D8;
    but9 = A0;
    butTime = A1;
    butPower = A2;
    butStop = A3;
    butStart = A4;
    
    // Configure the pins to be outputs
    pinMode(but0, OUTPUT);
    pinMode(but1, OUTPUT);
    pinMode(but2, OUTPUT);
    pinMode(but3, OUTPUT);
    pinMode(but4, OUTPUT);
    pinMode(but5, OUTPUT);
    pinMode(but6, OUTPUT);
    pinMode(but7, OUTPUT);
    pinMode(but8, OUTPUT);
    pinMode(but9, OUTPUT);
    pinMode(butTime, OUTPUT);
    pinMode(butPower, OUTPUT);
    pinMode(butStop, OUTPUT);
    pinMode(butStart, OUTPUT);
    
    // Initialize all outputs to be off
    digitalWrite(but0, LOW);
    digitalWrite(but1, LOW);
    digitalWrite(but2, LOW);
    digitalWrite(but3, LOW);
    digitalWrite(but4, LOW);
    digitalWrite(but5, LOW);
    digitalWrite(but6, LOW);
    digitalWrite(but7, LOW);
    digitalWrite(but8, LOW);
    digitalWrite(but9, LOW);
    digitalWrite(butTime, LOW);
    digitalWrite(butPower, LOW);
    digitalWrite(butStop, LOW);
    digitalWrite(butStart, LOW);
}


// This routine loops forever
void loop()
{

}

/* This function gets called whenever there is a matching API request
 * It will interpret the command string, parsing out individual
 * instructions for the cores to handle.
 * for example: ["t40,p5,start"]
 *      means run for 40 seconds, at 50% power
 *      it will be parsed out to [time cook, 4, 0, power, 5, start]
 *      that interpreted instr will then be sent to the appropriate cores
 */
int sendInstr(String rawCommands)
{
    int numCommands = countCharacters(rawCommands.c_str(), ',') + 1;
    String commands[numCommands];
    splitArgStringToArray(rawCommands, commands, ',');
    for(unsigned int i=0;i<arraySize(commands);i++){
        String cmd = commands[i];
        if (cmd.equals("start"))
            pressButton("start");
        else if (cmd.equals("stop"))
            pressButton("stop");
        else if (cmd.equals("clear")){
            pressButton("stop");
            pressButton("stop");
        }
        else if (cmd.charAt(0) == 't')
            cookForTime(cmd.substring(1, cmd.length()-1));
        else if (cmd.charAt(0) == 'p')
            setPower(cmd.substring(1, cmd.length()-1));
        else{
            return -1;
        }
    }
    return 1;
    
}

/* This function gets called whenever there is a matching API request
 * It will run a "comprehensive" test suite on the cores to make sure
 * all mappings are correct for the microwave.
 */
int mappingTest(String input){
    String instructions[] = {"time","9","8","5","4","power","7","start"};
    for(unsigned int i=0;i<arraySize(instructions);i++){
        pressButton(instructions[i]);
    }
    delay(2000);
    String instructions2[] = {"stop","stop","time","6","3","2","power","1","start"};
    for(unsigned int i=0;i<arraySize(instructions2);i++){
        pressButton(instructions2[i]);
    }
    delay(2000);
    pressButton("stop");
    pressButton("stop");
    
    return 1;
}

/* This function gets called whenever there is a matching API request
 * It will run a test on our open API to make sure that the encoded
 * instruction messages get interpreted correctly.
 */
int encodedInstrTest(String input){
    String encodedInstructions = "t9854|p7|start";
    sendInstr(encodedInstructions);
    delay(2000);
    encodedInstructions = "clear|t632|p1|start";
    sendInstr(encodedInstructions);
    delay(2000);
    encodedInstructions = "stop";
    sendInstr(encodedInstructions);
    
    return 1;
}


void cookForTime(String timeString){
    pressButton("time");
    findCookTimeButtonSequence(timeString);
    for(unsigned int i=0;i<timeString.length();i++){
        String timeDigit = String(timeString.charAt(i));
        pressButton(timeDigit);
    }
}

void findCookTimeButtonSequence(String &timeString){
    //first convert the String to an int
    unsigned int timeInSeconds = atoi(timeString.c_str());
    unsigned int minutes = timeInSeconds/60;
    unsigned int seconds = timeInSeconds%60;
    
    unsigned int timeVal;
    //concatenate the minutes and seconds you calculated to be correct
    //for microwave input
    if(seconds < 10){
        timeVal = concatenateNumbers(concatenateNumbers(minutes,0),seconds);
    }
    else
        timeVal = concatenateNumbers(minutes,seconds);
        
    //calculate the correct size for our buffer
    int strSize = (int)((ceil(log10(timeVal))+1)*sizeof(char));
    char timeAsCString[strSize]; //create our buffer
    sprintf(timeAsCString, "%d", timeVal); //fill the buffer
    timeString = timeAsCString; //finally correctly prepare our time string for input
}

void setPower(String powerVal){
    pressButton("power");
    pressButton(powerVal);
}

void pressButton(String but){
    if(but.toInt() >= 0 && but.toInt()<10){
        int buttonToPress;
        if(but.toInt() == 0)
            buttonToPress = but0;
        else if(but.toInt() == 1)
            buttonToPress = but1;
        else if(but.toInt() == 2)
            buttonToPress = but2;
        else if(but.toInt() == 3)
            buttonToPress = but3;
        else if(but.toInt() == 4)
            buttonToPress = but4;
        else if(but.toInt() == 5)
            buttonToPress = but5;
        else if(but.toInt() == 6)
            buttonToPress = but6;
        else if(but.toInt() == 7)
            buttonToPress = but7;
        else if(but.toInt() == 8)
            buttonToPress = but0;
        else if(but.toInt() == 9)
            buttonToPress = but0;
        else
            buttonToPress = -1;
        
        if(buttonToPress >= 0){
            digitalWrite(buttonToPress, HIGH);
            delay(BUTTON_DURATION);
            digitalWrite(buttonToPress, LOW);
        }
    }

    else if(but.equals("time")){
        digitalWrite(butTime, HIGH);
        delay(BUTTON_DURATION);
        digitalWrite(butTime, LOW);
    }
    else if(but.equals("power")){
        digitalWrite(butPower, HIGH);
        delay(BUTTON_DURATION);
        digitalWrite(butPower, LOW);
    }
    else if(but.equals("stop")){
        digitalWrite(butStop, HIGH);
        delay(BUTTON_DURATION);
        digitalWrite(butStop, LOW);
    }
    else if(but.equals("start")){
        digitalWrite(butStart, HIGH);
        delay(BUTTON_DURATION);
        digitalWrite(butStart, LOW);
    }
    delay(INSTRUCTION_DELAY);
}

//TY http://stackoverflow.com/questions/12700497/how-to-concatenate-two-integers-in-c
unsigned concatenateNumbers(unsigned int x, unsigned int y) {
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        
}

//TY http://stackoverflow.com/questions/4235519/counting-number-of-occurrences-of-a-char-in-a-string-in-c
unsigned int countCharacters(const char *str, char character)
{
    const char *p = str;
    unsigned int count = 0;

    do {
        if (*p == character)
            count++;
    } while (*(p++));

    return count;
}

//THANK YOU JESUS CHRIST
// http://community.spark.io/t/spark-function-limits/952/7
void splitArgStringToArray(String arguments, String *target, char delim){
    int numArgs = 0;
    int beginIdx = 0;
    int idx = arguments.indexOf(delim);

    while (idx != -1) {
    	String arg = arguments.substring(beginIdx, idx);
    	arg.trim();
    	target[numArgs] = arg;

    	beginIdx = idx + 1;
    	idx = arguments.indexOf(delim, beginIdx);
    	++numArgs;
    }

    // Single or last parameter
    String lastArg = arguments.substring(beginIdx);
    target[numArgs] = lastArg;
}