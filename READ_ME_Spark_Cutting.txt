Compile_ADUcut.C:

I have given numbers to sections of the code that I consider important. This text file should help you to understand what each section does.

1 : 1D histogram showing the frequency distribution of the mean ADU of each event. The one with 'v2' at the end is for spark events picked out using the hand scan mentioned in report.

2: 1D histogram showing the frequency distribution of the number of pixels above 200 ADU. The one with 'v2' at the end is for spark events picked out using the hand scan mentioned in report.

3: 2D histogram used for comparing the location of sparks against the location of non-sparks. The one with 'v2' at the end is for spark events picked out using the hand scan mentioned in report.

4: Function that reads the hand scan results from a CSV file named 'Hand_Scan' and places the data into two string vectors: one for run number and one for the event number.

5: Function used for analysing events from a run.

5.1: 2D histogram for the average bias event.

5.2: Variable that stores the value of the mean ADU for the bias event.

5.3: This if statement checks to see if there is a spark in the bias event. If there is a spark then that run is not analyzed. Perhaps this could be changed to just removing the specific event and using the remaining events to create the average bias event.

5.4: This divides all th bias events that have been added by the number of bias events to create the average bias event.

5.5: This variable is used to store the mean ADU of the previous event which is used in the consecutive mean  method for calculating the difference between the current event and the previous event's mean ADU.

5.6: This stores a copy of the current event. I believe it allows the current event to be manipulated easier than by directly trying to subtract the bias from the original version of the event.

5.7: Variable that stores the value of the mean ADU for the bias subtracted exposure event.

5.8: 1D histogram used to show the frequency distribution of the pixel intensities.

5.9: Variable used to store the number of pixels above 200 ADU for the specific event.

5.10: These if statements check if the pixel intensity is above 200 ADU and if it is then 1 is added to the counter described in 5.9.

5.11: This 'for' loop is where the hand scan data is used to separate the sparks from non-sparks in order to find the optimum removal technique. The 'else if' statement which checks if 'j==Run_Number.size()-1' is used to store non-spark events. It means that if none the strings in the vector strings match the event number and run number together then that event is a non-spark and is dealt with accordingly.

5.12: This section, which has been commented out, is used for applying the cut directly to the bias subtracted events without the use of a hand scan.

6.1: This is the function used to check through all of the runs and draw out the appropriate histograms mentioned in 1,2 and 3 of this text file.

6.2: This tries the run to see if it exists, if an error is caught (such as a non existent run number) then it continues to the next run. The try catch statements are perhaps not necessary since the function mentioned in 5 already checks to see if there is an existing file.

6.3: The rest of the code draws the histograms mentioned in 1,2 and 3. 



ADUcut_Graph.C:

This macro is used to analyze the choice of looking at the number of pixels above 200 ADU compared to mean+Number_of_std_dev ADU or say 180 ADU. It draws graphs that show how the number of pixels varies as event number increases.



Eff_Pur_Graph.C:

This macro draws a graph that compares the efficiency with the purity for different removal points. This graph is used to show how efficiency changes with purity. The size of the 'Double' arrays depends on how many mean ADU points are used. The text file used for this is Eff_Pur_Cut3.txt.



Eff_Pur_Plot.C:

This macro draws a graph that compares the efficiency with purity for different removal points. This graph is used to determine the optimal removal point. The text file used for this is Eff_Pur_Cut2.txt.



Consecutive_Mean_Graph.C:

This macro is used for drawing graphs that show how the mean ADU changes as the event number increases. The size of xcam and ycam arrays depend on the number of bias events and need to be changed if the number of bias events changes. Likewise for the bias subtracted events. The array size should change automatically but I was unable to find a way to do this.