

void init1()
{
  //Serial
  Serial.begin(serial_speed);
  bug_print("start init- ");

  //LCD
  lcd.begin (16, 2);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  bug_print(" -lcd- ");
  
  lcd.clear();
  lcd.print("init");
  

  //Caliper interface
  //pinMode(data_pin, INPUT);
  //pinMode(clock_pin, INPUT);
  //digitalWrite(data_pin, LOW);
  //digitalWrite(clock_pin, LOW);
  attachInterrupt(interrupt_pin, dataReceive, FALLING);
  bug_print(" -caliper- ");

  //SD
  pinMode(sd_slct_pin, OUTPUT);

  if (!SD.begin(sd_slct_pin))
  {
    bug_println(" - SD init failed!");
    lcd.clear();
    lcd.print("SD card missing!");
    lcd.setCursor(0,1);
    lcd.print("insert & reboot");
    while (1);
  }

  /*if(SD.exists("testfile.txt"))
  {
    bug_println(" - File init failed! File already exsists!");
    while (1);
  }
  
  File test_file;
  test_file = SD.open("testfile.txt", FILE_WRITE);
  
  if(!test_file)
  {
    bug_println(" - File init failed! Could not open file!");
    while (1);
  }
  
  test_file.println("test 123");
  test_file.close();
  delay(50);
  
  test_file.open();
  bug_println(test_file.read());
  bug_println("test");
  if(!(test_file.read() == "test 123"))
  {
    bug_println(" - File init failed! Could not read from file and/or verify!");
    while (1);
  }
  
  test_file.close();
  delay(50);
  
  if(!SD.remove("testfile.txt"))
  {
    bug_println(" - File init failed! Could not remove file!");
    while (1);
  }
  */
  bug_print(" -sd- ");

  //caliper button
  pinMode(caliper_button_pin, INPUT);
  digitalWrite(caliper_button_pin, HIGH);

  //char key = cKeypad.getKey();
  bug_print(" -buttons- ");



  bug_println(" - end init");

}


void welcome()
{
  bug_print("start welcome- ");

  lcd.clear();

  lcd.print(" COOPER-CALIPER ");
  lcd.setCursor(0, 1);
  delay(500);
  lcd.print("Hold for Calibr.");

  uint32_t time1 = millis();
  while (millis() - time1 <= 1200)
  {
    if (digitalRead(caliper_button_pin) == 0 || !(0 == (uint8_t)cKeypad.getKey()))
    {
      calibration();
    }
    delay(1);
  }

  bug_println(" - end welcome");
}

