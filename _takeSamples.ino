
uint8_t back_counter;
uint32_t last_back_press; 
uint32_t last_press = 0;

bool press_again = true;

bool change_tree = false;
bool change_area = false;

bool change_tree_for_real = false;
bool change_area_for_real = false;

bool jump_shit = false;

void takeSamples()
{
  
  if(init_take_sample)
  {
    initTakeSample();
  }
  
  static uint32_t display_counter;
  if(millis() - display_counter >= 200)
  {
    display_counter = millis();
    displayTakeSamples();
  }
  getKeysTakeSamples();
  work_take_samples();
}

void work_take_samples()
{
  if(change_tree)
  {
    change_tree_again:
    
    uint32_t start_of_thing = millis();
    
    while(millis() - start_of_thing <= 2000)
    {
      if(change_tree)
      {
        change_tree = false;
        if(change_tree_for_real)
        {
          tone(buzz_pin, 700, 50);
          change_tree_for_real = false;
          if(tree_slct < number_of_trees)
          {
            tree_slct ++;
          }
          else 
          {
            tree_slct = 0;
          }
        }
        lcd.clear();
        lcd.print("Current Tree is:");
        lcd.setCursor(1,1);
        lcd.print(tree[tree_slct]);
      }
    
      if('3' == cKeypad.getKey()) 
      {
        change_tree = true;
        change_tree_for_real = true;
        
        goto change_tree_again;
      }
    }
  }
  if(change_area)
  {
    change_area_again:
    
    uint32_t start_of_thing = millis();
    
    while(millis() - start_of_thing <= 2000)
    {
      if(change_area)
      {
        change_area = false;
        if(change_area_for_real)
        {
          tone(buzz_pin, 600, 50);
          change_area_for_real = false;
          if(area_slct < number_of_areas)
          {
            area_slct ++;
          }
          else 
          {
            area_slct = 0;
          }
        }
        lcd.clear();
        lcd.print("Current area is:");
        lcd.setCursor(0,1);
        lcd.print(area[area_slct]);
      }
    
      if('1' == cKeypad.getKey()) 
      {
        change_area = true;
        change_area_for_real = true;
        
        handleLeaveTakeSamples();
        if(jump_shit) return;
        
        goto change_area_again;
        
      }
    }
  }
}

void getKeysTakeSamples()
{
  if(digitalRead(caliper_button_pin) == 0)
  {
    if(press_again)
    {
      sample_taken = true;
      
      press_again = false;
      last_press = millis();
    }
    if(millis() - last_press >= 600)
    {
      press_again = true;
    }    
  }
  else 
  {
    if(millis() - last_press >= 100)
    {
      press_again = true;
    }
  }

  
  char key = cKeypad.getKey();
  
  if(key == '3')
  {
    change_tree = true;
    
    tone(buzz_pin, 700, 50);
  }
  else if(key == '2')
  {
    sample_taken = true;
  }
  else if(key == '1')
  {
    change_area = true;
    
    tone(buzz_pin, 500, 100);
    
    handleLeaveTakeSamples();
  }
}

void initTakeSample()
{
  lcd.clear();
  lcd.print("Working in File:");
  lcd.setCursor(0,1);
  lcd.print(" ");
  lcd.print(file1_name);
  delay(200);

  bool stopper_init_take_samples = true;
  while(stopper_init_take_samples)
  {
    char key = cKeypad.getKey();
    if(key == '1')
    {
      lcd.clear();
      lcd.print("Returning!");
      tone(buzz_pin, 300, 50);
      delay(1000);
      menu_slct_x = 0;
      back_counter = 0;
      init_take_sample = true ; 
      return;
    }
    else if(key == '2')
    {
      stopper_init_take_samples = false;
      lcd.clear();
      lcd.print("Press caliper");
      lcd.setCursor(0,1);
      lcd.print("to take sample!");
      tone(buzz_pin, 600, 50);
    }
  }
  file1 = SD.open(file1_name, FILE_WRITE);
  delay(50);
  
  file1.println(" ");
  file1.print("Number of Samples, Diameter, Type of Tree,");
  file1.println("Area, Time ");
  file1.flush();

  delay(800);
  init_take_sample = false; 
}

void displayTakeSamples()
{
  if(!sample_taken)
  {
    lcd.clear();
    lcd.print("Caliper: ");
    lcd.print(value - cal_offset);
    lcd.setCursor(0,1);
    lcd.print("#");
    lcd.print(sample_counter);
    lcd.print(" Last: ");
    if(prev_sample == 0.0)
    {
      lcd.print("--.--");
    }
    else 
    {
      lcd.print(prev_sample);
    }
  }
  else 
  {
    lcd.clear();
    lcd.print("Saving Sample!");
    lcd.setCursor(0,1);
    tone(buzz_pin, 600, 100);
    prev_sample = getMess(5,1);
    tone(buzz_pin, 700, 100);
    lcd.print("Sample: ");
    lcd.print(prev_sample);
    saveSample(prev_sample);
    delay(200);
    sample_taken = false;
    sample_counter ++;
  }
}

/*void saveSample()
{
  current_sample = getMess(5,1);
  file1.print(sample_counter);
  file1.print("  ,  ");
  file1.print(current_sample);
  file1.print("  ,  ");
  file1.print(tree[tree_slct]);
  file1.print("  ,  ");
  file1.print(millis() / 1000);
  file1.println();
  file1.flush();
  delay(100);
}*/

void saveSample(float sample_to_save)
{
  file1.print(sample_counter);
  file1.print("  ,           ");
  file1.print(sample_to_save);
  file1.print("  ,           ");
  file1.print(tree[tree_slct]);
  file1.print("  ,           ");
  file1.print(area[area_slct]);
  file1.print("  ,           ");
  file1.print(((float) millis()) / 1000, 2);
  file1.println();
  file1.flush();
  delay(100);
}

void handleLeaveTakeSamples()
{
  if(back_counter < 2 && millis() - last_back_press <= 300)
  {  
    back_counter ++;
  }
  else if(millis() - last_back_press > 300)
  {
    back_counter = 0;
  }
  
  if(back_counter >= 2)
  {
    leaveTakeSamples();
  }
  
  last_back_press = millis();
}

void leaveTakeSamples()
{
  menu_slct_x = 0;
  back_counter = 0;
  sample_counter = 0;
  init_take_sample = true ; 
  lcd.clear();
  lcd.print("Saving data");
  lcd.setCursor(0,1);
  lcd.print("to SD card");
  file1.close();
  tone(buzz_pin, 600, 300);
  delay(350);
  tone(buzz_pin, 500, 300);
  delay(350);
  jump_shit = true;
}

