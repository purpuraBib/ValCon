State controllers addition using State Controller Extension<br><br>
This adds
- **ValSet** - Set a value in a P-relative address<br>
  Disp = (displacement from P)<br>
  Value = (value)
- **ValAdd** - Add a value in a P-relative address<br>
  Disp = (displacement from P)<br>
  Value = (value)
- **StringValSet** - Write a string in a P-relative address<br>
  Disp = (displacement from P)<br>
  Text = (string)<br>
  Terminate = (1 to add 0x00 as a string terminator)
- **ValSetPointer** - Set a value to a specified pointer<br>
  Pointer = (pointer in a certain format like "\[\[0x4b5b4c\]+0xb758\]+0xe24")<br>
  Params = (parameters for format selectors used in Pointer)<br>
  Value = (value)
