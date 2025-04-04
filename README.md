# terminal-ads  
One of the worst ideas ever: intrusive banner ads in your terminal.

## About  
Ever felt like your terminal was too clean? No popups, no flashing banners, no annoying interruptions like the current state of the Web in 2025? Time to fix that.  

**terminal-ads** is (hopefully) a proof-of-concept that brings the horrors of modern advertising to the only place that was still safe and ad-free: **your terminal.** This Windows program ensures that persistent, intrusive, useless and distracting banners are always displayed at the top of your terminal window. Fortunately, there is no Linux version yet so you're safe over there (for now).

Because why should terminals be an exception to the rest of the modern world?  

## Features  
- 📌 **Persistent Banner Ads**: Always at the top, whether you scroll or clear the screen.  
- 🚫 **Unskippable**: The ads stay there for the entire terminal. Forever.
- ⚙️ **Customizable Ad Intrusiveness**: Ability to choose how much suffering you want.
- 🤡 **Completely Unnecessary**: It's not a big surprise.  

## Why?  
Because every space should be monetized. Every blank space is an opportunity for advertising. And also, because nobody asked for this.  

## Usage

`ads [1 | 2 | 3 | 4]`
The different numbers indicate the level of intrusiveness:
- **Free tier (1)**
  - Persistent ads displayed every 0.5 seconds when the screen is cleared
  - Console title changes to the text displayed on the banner
- **Basic tier (2)**
  - Persistent ads displayed every second when the screen is cleared
- **Premium tier (3)**
  - Inserts an intrusive ad once, no persistence, no flashing colors.
- **Premium+ tier (4)**
  - _Recommended option:_ doesn't insert useless junk into your terminal, and does absolutely nothing.

The file `ad.lst` contains all the messages that will get displayed inside the banner. At the moment only single line advertisements are supported, with a single advertisement per line such as:
```
First ad! Don't use this program
Second ad! You really should not use this program
Try out Newtrodit, the best console text editor ever made!
```

Additionally, if you're feeling **extra** silly, you can run `ads --autorun` and let the magic begin. What does this do? Once you open a new CMD instance and see an obnoxious ad at the top you'll wonder how that happened. You are warned.

## Installation

You really shouldn't use this anywhere. But if you really want to, here are the steps:

1. Clone this abomination:  
   ```sh
   git clone https://github.com/anic17/terminal-ads.git  
   cd terminal-ads
   ```
2. Build it with your Windows compiler.

3. Run it. And regret it.

# Screenshots
###### terminal-ads running with Free tier, how intrusive is this?
![image](https://github.com/user-attachments/assets/82bc28ae-14d2-490f-9648-1c0fd498efe8)



**Copyright &copy; 2025 anic17 Software**
