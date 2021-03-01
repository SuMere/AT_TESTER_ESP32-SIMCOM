# AT TESTER

It is possible to test AT commands passing the desidered command to <code>void test_at(const char* command)</code> function in main.

### :bangbang:ACHTUNG:bangbang:

Do not forget to put "AT+" before the actual command and "\r\n" after the command.

## Interactive shell

The firmware gives the opportunity to send AT commands without embedding them in the code.
The shell is activated calling <code>init_uart()</code> function.

Once ready message is shown it is possible to write the AT command press enter to add /n/r chars then write the '@' char, this is the trigger that send the command to the modem.