Greetings!

On behalf of the entire team, I would like to welcome you to SIC SOLutions, Inc! There were a lot of applicants for the systems programmer job, but in the end, we hired you because you passed Mr. Piersall's System Software course. 

Under normal circumstances, we like to provide our new hires some time to learn about our products and gain some organizational know-how. Unfortunately, we just signed a big contract with a tight deadline and need you to deliver a device driver for our newest customer.  We know you are up to the challenge!

We just signed a contract with the Burnt-to-a-crisp oven company.  They make simple-to-use ovens for the consumer products market.  They have awarded us the contract to provide an embedded controller, based upon the SIC machine, for 10,000 of their ovens. We already have teams working on the SIC programs to monitor the temperature and modes of the oven.  What we need is a SIC program that implements a time display on the oven's LCD display. Unfortunately, Burnt-to-a-crisp does not have the on-staff engineering task to complete this (they all failed Mr. Piersall's system software course). Burnt-to-a-crisp is depending on us, and we are depending on you.

To further complicate matters, Burnt-to-a-crisp already purchased 10,000 timer circuits and wants the circuits hard-wired to the SIC. (They signed a contract with the supplier and so the devices cannot be returned). Ouch! So here are your constraints:

 

1. The timer circuit is hardwired to the last word of addressable memory in the SIC system. The timer counts the number of seconds which have elapsed since midnight and places that value in the last word of addressable memory.  The timer is a self-contained circuit that automatically resets itself at midnight.  If the consumer changes the time on the clock, the timer circuit changes its output. Your program does not need to do that or worry about the mechanics of the time changes. Your program needs to output the correct time of day based upon the value in the last word of memory.

2. The consumer does have the option of having the clock display in 12Hour or 24Hour format. If the user wants to see the 24-hour format, the timer circuit sets the most significant BIT in the last word of addressable memory to 1. For 12 hour format, the bit is set to zero.

3. Our engineers have already written a "monitor" program that calls all of the different routines needed for the oven to operate correctly. The monitor program will call your program at regular intervals so that the time displayed on the oven is correct. 

4. The oven's LCD panel is hardwired to device number 64 on the SIC. The display is configured to display the characters from the ASCII bytes it receives. 

If it is 1:16PM, your program should write the following to the display device:

24 hour time:   1316  IE:  HHMM    where HH is the hour and MM is the minute

12 hour time:   0116P  IE: HHMMT where HH is the two-digit hour, MM is two digit minute and T is P - PM A - AM

5. Our engineers are still working on their monitor program, and are confident that you can use the last 1K bytes of memory. If you can write the program (and have it produce correct results) using less memory we will award you a bonus (extra points on the assignment)

 

We need two files from you on this.

1. The sic program. Please call it CLOCK, and it needs to be in a file called clock.sic. We know this could be easier with the XE version, but using the XE is cost-prohibitive to the customer. We have to do this in SIC assembly. 

2. A documentation file.  Please tell us how many bytes of memory your program needs and where you had it load by default (we will need to call it from the monitor). If you needed less than 1k to get this done, great!   Move it closer to the end of addressable memory so we have more space available should the monitor or other programs need it.

Please create a ZIP archive containing these two files.

Please submit your work to our engineering team via System_Software_Project3 on the class server.

This program is due on November 4, 2021 at 11:59PM EST.
