void error(uint8_t error_code)
{
  lcd.clear();
  lcd.setBacklight(1);
  lcd.print("*****ERROR******");
  bug_print("------ERROR------    ");
  lcd.setCursor(0, 1);
  switch (error_code)
  {
    case 1:
      lcd.print(" MESS TIMEOUT");
      bug_println("MESS TIMEOUT");
      break;
    case 2:
      lcd.print("Testfile exists");
      bug_println("Testfile already exists");
      break;
    case 3:

      break;

    default:
      lcd.print(" GENERAL ERROR!");
      bug_println("GENERAL ERROR!");
      break;
  }
  while (1);
}


void calibration()
{
  bug_print(" calibration start ");
  bool exit_cal = true;

  lcd.clear();
  lcd.print("--CALIBRATION---");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Close Caliper,");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Close Caliper,");
  lcd.setCursor(0, 1);
  lcd.print("press enter!     ");
  //delay(1000);

  uint32_t timer1 = millis();
  while (millis() - timer1 <= 10000)
  {
    if (digitalRead(caliper_button_pin) == 0 || '2' == cKeypad.getKey())
    {
      exit_cal = false;
      break;
    }
    else if ('1' == cKeypad.getKey() || '3' == cKeypad.getKey())
    {
      exit_cal = true;
      break;
    }
  }

  if (exit_cal)
  {
    lcd.clear();
    lcd.print("Cali. error");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("end cali.");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("return to main...");
    delay(2000);
    return ;
  }
  else if (!exit_cal)
  {
    start_of_cali:

    float offset_prev = 0.0;

    lcd.clear();
    lcd.print("Start cali");
    lcd.setCursor(0, 1);
    lcd.print("Sample: ");

    for (int i = 0; i < 5; i++)
    {
      lcd.setCursor(9, 1);
      lcd.print(i);
      offset_prev += getMess(7, 0);
      bug_println(offset_prev);
      //delay(1000);
    }
    offset_prev /= 5.0;

    lcd.clear();
    lcd.print("Got Offse.: ");
    lcd.print(offset_prev);
    lcd.setCursor(0, 1);
    lcd.print("press to redo");
    delay(1000);
    uint32_t time12 = millis();
    uint32_t time21 = millis();

    while (millis() - time12 <= 2100)
    {
      if (digitalRead(caliper_button_pin) == 0 || !(0 == (uint8_t)cKeypad.getKey()))
      {
        bug_print("redo calibration");
        goto start_of_cali;
      }
      delay(1);
      if (millis() - time21 >= 690)
      {
        time21 = millis();
        lcd.print(".");
      }

    }
    delay(500);

    cal_offset = offset_prev;
    lcd.clear();
    lcd.print("Saving calibration");
    bug_println("saving calibration data");
    lcd.setCursor(0, 1);
    lcd.print(cal_offset);
    delay(1000);
  }
}


void limitMenu()
{
  if (menu_slct_x < 0) menu_slct_x = 0;
  else if (menu_slct_x > 1) menu_slct_x = 1;

  if (menu_slct_y < 0) menu_slct_y = 0;
  else if (menu_slct_y > 2) menu_slct_y = 2;
  //bug_print(menu_slct_x);
  //bug_print("   y  ");
  //bug_println(menu_slct_y);
}

