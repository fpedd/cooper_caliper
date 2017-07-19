volatile void dataReceive()
{
  uint8_t raw_bit = 0;
  if(PING & (1 << data_pin_low_level))
  {
    //bug_print("1 ");
    raw_bit = 1;
  }
  if (millis() - last_data >= 50)
  {
    value = (float)raw_value;
    value = ((value / 2) * (float)sign_calip) / 100.0;
    //Serial.print(raw_value, BIN);
    //bug_print("   ");
    //bug_println(value);
    bit_pos_counter = 0;
    raw_value = 0;
    ready_flag = true;
    sign_calip = 1;
  }
  else if (bit_pos_counter < 21)
  {
    raw_value |= raw_bit << bit_pos_counter;
  }
  else if (bit_pos_counter == 21)
  {
    if (raw_bit == 1)
    {
      sign_calip = -1;
    }
  }
  last_data = millis();
  bit_pos_counter ++;
}

float getMess(uint8_t number_of_samples, bool offset_en)
{
  uint32_t time_it_takes = millis();

  float mess_holder[number_of_samples];
  memset (mess_holder, 0.0, sizeof(mess_holder));

  attachInterrupt(interrupt_pin, dataReceive, FALLING);
  delay(1);

  ready_flag = false;
  uint32_t watchdog = millis();
  while (!ready_flag && millis() - watchdog < 2000);
  if (millis() - watchdog >= 2000) error(1);

  for (int i = 0; i < number_of_samples; i ++)
  {
    ready_flag = false;
    uint32_t watchdog = millis();
    while (!ready_flag && millis() - watchdog < 2000);
    if (millis() - watchdog >= 2000) error(1);
    mess_holder[i] = value;

  }
  if (number_of_samples = 1)
  {
    if (offset_en)
    {
      mess_holder[0] -= cal_offset;
    }
    return  mess_holder[0];
  }
  detachInterrupt(interrupt_pin);

  uint8_t max_index = 0;
  uint8_t min_index = 0;
  for (int i = 1; i < number_of_samples; i ++)
  {
    if (mess_holder[min_index] > mess_holder[i])
    {
      min_index = i;
    }
    if (mess_holder[max_index] < mess_holder[i])
    {
      max_index = i;
    }
  }
  float value_to_return = 0.0;

  for (int i = 0; i < number_of_samples; i ++)
  {
    if (i != max_index && i != min_index)
    {
      value_to_return += mess_holder[i];
    }
  }
  if (min_index != max_index)
  {
    value_to_return /= (((float) number_of_samples) - 2.0);
  }
  else
  {
    value_to_return /= (((float) number_of_samples) - 1.0);
  }

  if (offset_en)
  {
    value_to_return -= cal_offset;
  }

  return value_to_return;

}

