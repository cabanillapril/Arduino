
  /* CSE Final Project - Integer
  *  Group Members: Cabanilla, April Anne 
  *                Poderoso, Marwin
  *                Quilop, Nestor
  * CYS: BSCS - 2A
  * This program generates musical notes using predefined integer frequencies, 
  * similar to how early computers and gaming systems synthesized sound.
  * 
  */
   
  // Define input and output pins
  #define ledPins2 {5, 6} // 2 LEDs for sequential blinking
  #define buzzPin 3 // Buzzer
  #define buzzBtnPin 13 // Button to control buzzer
  #define potPin A0 // Potentiometer

  // Define musical notes
  #define noteC4  262
  #define noteD4  294
  #define noteE4  330
  #define noteF4  349
  #define noteG4  392
  #define noteA4  440
  #define noteB4  494
  #define noteC5  523 
  #define noteD5  587
  #define noteE5  659
  #define noteF5  698
  #define noteG5  784
  #define noteA5  880
  #define noteB5  988
  #define noteC6  1047
  #define rest     0

  // "Do Re Mi Fa So La Ti Do" Melody
  int melody2[] = {
      noteC4, noteD4, noteE4, noteF4, noteG4, noteA4, noteB4, noteC5, rest, // Ascending
      noteB4, noteA4, noteG4, noteF4, noteE4, noteD4, noteC4 // Descending
  };

  int duration2[] = {
      500, 500, 500, 500, 500, 500, 500, 500, 1000,// Ascending
      500, 500, 500, 500, 500, 500, 500 // Descending
  };


  // "Twinkle Twinkle Little Star"
  int melody1[] = {
      noteC4, noteC4, noteG4, noteG4, noteA4, noteA4, noteG4,
      noteF4, noteF4, noteE4, noteE4, noteD4, noteD4, noteC4,
      noteG4, noteG4, noteF4, noteF4, noteE4, noteE4, noteD4,
      noteG4, noteG4, noteF4, noteF4, noteE4, noteE4, noteD4,
      noteC4, noteC4, noteG4, noteG4, noteA4, noteA4, noteG4,
      noteF4, noteF4, noteE4, noteE4, noteD4, noteD4, noteC4
  };

  int duration1[] = {
      500, 500, 500, 500, 500, 500, 1000, 
      500, 500, 500, 500, 500, 500, 1000, 
      500, 500, 500, 500, 500, 500, 1000, 
      500, 500, 500, 500, 500, 500, 1000, 
      500, 500, 500, 500, 500, 500, 1000, 
      500, 500, 500, 500, 500, 500, 1000
  };

  // "Mary Had a Little Lamb"
  int melody3[] = {
      noteE4, noteD4, noteC4, noteD4, noteE4, noteE4, noteE4,
      noteD4, noteD4, noteD4, noteE4, noteG4, noteG4,
      noteE4, noteD4, noteC4, noteD4, noteE4, noteE4, noteE4,
      noteE4, noteD4, noteD4, noteE4, noteD4, noteC4
  };
  
  int duration3[] = {
      300, 300, 300, 300, 300, 300, 600,
      300, 300, 600, 300, 300, 600,
      300, 300, 300, 300, 300, 300, 400,
      300, 300, 300, 300, 300, 600
  };


  // "Star Wars Theme"
  int melody4[] = {
      noteA4, noteA4, noteA4, noteF4, noteC5, noteB4, 
      noteA4, noteF4, noteC5, noteB4, noteA4, noteF4, 
      noteC5, noteB4, noteA4, noteF4, noteC5, noteB4, 
      noteA4, noteA4, noteA4, noteF4, noteC5, noteB4
  };

  int duration4[] = {
      400, 400, 400, 400, 400, 400, 
      400, 400, 400, 400, 400, 400, 
      400, 400, 400, 400, 400, 400, 
      400, 400, 400, 400, 400, 400
  };

  // "Ode to Joy" 
  int melody5[] = {
      noteE4, noteE4, noteF4, noteG4, noteG4, noteF4, noteE4, noteD4,
      noteC4, noteC4, noteD4, noteE4, noteE4, noteD4, noteD4,
      noteE4, noteE4, noteF4, noteG4, noteG4, noteF4, noteE4, noteD4,
      noteC4, noteC4, noteD4, noteE4, noteD4, noteC4, noteC4
  };

  int duration5[] = {
      500, 500, 500, 500, 500, 500, 500, 600,  
      500, 500, 500, 500, 600, 400, 1000,  
      500, 500, 500, 500, 500, 500, 500, 600,  
      500, 500, 500, 500, 600, 400, 1000  
  };


  int *melodies[] = {melody1, melody2, melody3, melody4, melody5};
  int *durations[] = {duration1, duration2, duration3, duration4, duration5};
  int melodyLengths[] = {42, 16, 26, 24, 30}; // Length for melodies

  int currentSong = 0;
  bool isPlaying = false;

  void setup() {
      pinMode(buzzPin, OUTPUT);
      pinMode(buzzBtnPin, INPUT_PULLUP);
      pinMode(potPin, INPUT);
      int ledPins[] = ledPins2;
      for (int i = 0; i < 2; i++) {
          pinMode(ledPins[i], OUTPUT);
      }
  }
  
  void loop() {
      handleBuzzer();
  }

  void handleBuzzer() {
      static bool btnPressed = false;
      static unsigned long btnTime = 0; // To track the start time of the button press
      const unsigned long longPress = 500; // 500 milliseconds for long press
      const unsigned long debounceDelay = 100; // 50 milliseconds for debounce

      // Read the current button state
      bool currentBtnState = digitalRead(buzzBtnPin);

      if (currentBtnState == LOW && !btnPressed) {
          // Button was just pressed
          btnPressed = true;
          btnTime = millis(); // Record the time the button was pressed
          delay(debounceDelay); // Debounce delay
      }

      if (currentBtnState == HIGH && btnPressed) {
          // Button was just released
          btnPressed = false;
          delay(debounceDelay); // Debounce delay
          unsigned long pressDuration = millis() - btnTime; // Calculate how long the button was pressed

          if (pressDuration < longPress) {
              // Short press: Change song
              currentSong = (currentSong + 1) % 5; // Change to the next song
              isPlaying = true; // Start playing the new song
              Serial.print("Song changed to: ");
              Serial.println(currentSong);
          } else {
              // Long press: Stop music
              isPlaying = false; // Stop playing
              noTone(buzzPin); // Ensure the buzzer is off
              Serial.println("Music stopped");
          }
      }

      // If a song is currently playing, call playSong
      if (isPlaying) {
          playSong(currentSong);
      }
  }
  // Play the songs
  void playSong(int songIndex) {
      int *melody = melodies[songIndex];
      int *duration = durations[songIndex];
      int length = melodyLengths[songIndex];
      int ledPins[] = ledPins2;

      for (int i = 0; i < length && isPlaying; i++) {
          if (digitalRead(buzzBtnPin) == LOW) {
              noTone(buzzPin);
              isPlaying = false; // Stop playing when the button is pressed
              Serial.println("Song stopped by button press.");
              return;
          }

          int noteDuration = duration[i];
          // ADjust brightness
          int brightness = analogRead(potPin) / 4;
         
          // Produce sound
          tone(buzzPin, melody[i], noteDuration);

          // Blinking Leds
          for (int j = 0; j < 2; j++) {
              analogWrite(ledPins[j], brightness);
              delay(noteDuration / 3); // Blink for one third of the note duration
              analogWrite(ledPins[j], 0);
          }

          noTone(buzzPin);
          delay(50);
      }

      isPlaying = false;
  }
