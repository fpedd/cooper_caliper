
void background()
{
  static uint32_t background_counter; 
  if(millis() - background_counter >= 1000)
  {
    background_counter = millis();
    check_voltage();
  }
}

void check_voltage()
{
  
  voltage_too_low:

  float voltage_sample = 0;
  for(int i=0; i<3; i++)
  {
    voltage_sample += (float) analogRead(vin_pin);
  }
  
  voltage_sample /= 3.0;
  voltage_sample *= (5.0 / 1023.0);

  if(voltage_sample <= voltage_threshold && !ignore_voltage)
  {
    lcd.clear();
    lcd.print("Voltage too low:");
    lcd.setCursor(0,1);
    lcd.print("Please charge me!");
    delay(500);
    if(cKeypad.getKey()) 
    {
      ignore_voltage = true;
      tone(buzz_pin, 700, 50);
      delay(500);
    }
    //bug_println(voltage_sample);
    
    goto voltage_too_low;
  }
  

}

