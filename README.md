# trakboxDue
Information on Jamsat/TAPR Trakbox Hardware and Software. Work on new version of Trakbox on newer hardware.

IMPORTANT UPDATE! Originally, this was going to be a project to emulate the older trakbox hardware including CPU so that older firmware image would operate on newer ARM based hardware. This was due to the fact I could not make contact with original Author and there was concern that the original software was lost. However, after one more try to find the Author, we have had success! Dr. Sasato JA6FTL sent me the following email on June 20th of 2021.
------------------------------------------------------------------
Hello OM,
I had phone call from Wakitasan,JE9PEL about my old C-source code of
TrakBOx.
It has flown many time after the last TrakBox EPROM release.
My TrakBOx development system was missing due to old PC failure.
After searching the PC from my junk box, finally ang fortunately, I found
The broken PC and extract IDE hard disk.
Here I enclosed the C-source code for 8052, 80C320, 80C251 CPU If you need
the DEFINE, INCLUDE files for compiling I will forward you The total system
files of compiler set.

The enclosed ZIP file has
W3IWI's original tracking BASIC code which I implemented for C The latest
source code  ETC.

I hope the code is helpful for your project.
BTW, what is the CPU you intend to implement?

73s  de  JA6FTL
------------------------------------------------------------------

He did a lot of work to recover the source code of the original Trakbox! 
Once I confirm it is ok to post publicly here I will do so and start on
recompiling it for newer ARM based hardware. I have decided to use an
older dev kit I had MCBSTR9 to start since it has the same type of lcd
originally used in the original TRAKBOX and also has a RTC and lots of
i/o and serial ports which already have the rs-232 drivers installed.

![mcbstr9](https://user-images.githubusercontent.com/20524747/122818201-c1e27e80-d28d-11eb-8ae6-ee3afeb111ed.jpg)

Thanks to Martin W6LHI/G8LHI who found this github page and contacted me to tell me he found his old original trakbox has
sent it to me to use for reference! It's in great shape! 
![IMG-2851](https://user-images.githubusercontent.com/20524747/170761312-d44bcdd4-4b72-4647-9db8-3784334cf27e.JPG)

I have successfully compiled the original source code for trakbox. I will burn an eprom and test it in Martins unit to validate
that it does work. After which I will update the data in the code to bring it up to 2021-2030 and publish a new rom image for
anyone wanting to still use their units. Then I will port this code to a newer processor and see how that works out. The
goal is to replace the trakbox hardware with a more modern faster and lower power device.

Check back for updates. 73
