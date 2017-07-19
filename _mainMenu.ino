


void mainMenu()
{
  static uint32_t display_counter;
  if(millis() - display_counter >= 200)
  {
    display_counter = millis();
    displayMainMenu();
  }
  getKeysMainMenu();
  //bug_println(display_counter);
}

void displayMainMenu()
{
  lcd.clear();
  if (menu_slct_y == 0)
  {
    lcd.print("*COOPER-CALIPER*");
    lcd.setCursor(0,1);
    lcd.print("Take Samples");
  }
  else if (menu_slct_y == 1)
  {
    lcd.print("Take Samples");
    lcd.setCursor(0,1);
    lcd.print("Edit SD-Card");
  }
  else if (menu_slct_y == 2)
  {
    lcd.print("Edit SD-Card");
    lcd.setCursor(0,1);
    lcd.print("Settings");
  }
  lcd.setCursor(14,1);
  lcd.print("<-");

}

void getKeysMainMenu()
{
  char key = cKeypad.getKey();
  if(key == '3')
  {
    if(menu_slct_y > 0)
    {
      menu_slct_y --;
      tone(buzz_pin, 700, 50);
    }
    else
    {
      //tone(buzz_pin, 300, 50);
    }
  }
  else if(key == '2')
  {
    if(menu_slct_x < 1)
    {
      menu_slct_x ++;
      tone(buzz_pin, 600, 50);
    }
    else
    {
      //tone(buzz_pin, 300, 50);
    }
  }
  else if(key == '1')
  {
    if(menu_slct_y < 2)
    {
      menu_slct_y ++;
      tone(buzz_pin, 500, 50);
    }
    else
    {
      //tone(buzz_pin, 300, 50);
    }
  }
}

