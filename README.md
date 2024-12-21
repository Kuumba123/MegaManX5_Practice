# MegaMan X5 Practice
This is a hack to assist with practicing speedruns of the PSX version of MegaMan X5. It primarly adds save states which are not normally possible on PSX games due to the nature of the Disc Games.

## Compile
NOTE these steps might change in the future so keep that in mind when fetching the newest version of this repro.
1st your gonna need a total of 3 repros (including this one)
 * My Fork of Red-Hot's PSX Modding Repro https://github.com/Kuumba123/psx-modding-toolchain
 * My MMX5 PSX Repro https://github.com/Kuumba123/MegaManX5_PS1_Modding

Once you have all 3 go back to the github with Red-Hot's PSX modding repro and following the instructions there. If you can compile any of the examples included in that repro you should be able to compile this one.

Then put the MegaMan X5 PSX repro inside the `games` folder and then this repro inside your `mods` folder and put your MegaMan X5 ISO files inside the `build` folder (must be named `MMX5.cue` and `MMX5.bin`).



Once your done setting up all the files go back to the MMX5 Practice repro and run the MOD.BAT file. The only options that you should really be interested in are the option `1` , `3` and `4`. 1st you should do option 3 to extract the game files for editing then option 1 to compile then option 4 to actually build the game disc. But before you you use option 4 copy the `ROCK_X5.DAT` file and put it in the `arc/american` folder.
Then run the american.bat file witch will run the datwrite python script (only need to do this once) . Then you can run option 4 and you should see the new ISO files in the build folder.