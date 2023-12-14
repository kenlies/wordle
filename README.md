# wordle
## This was an optional duo (rush) project at Hive Helsinki

This project was fairly easy to make, infact I made a working product within a couple of hours alone before I had any discussion with my partner.
But that version was pretty scuffed, and we sat down with my partner and created (he) a better one with python. It still works within the terminal so its quite primitive in a sense but I'm happy how it turned out. After that, I made the same impl using C with some improvements. I've included both versions in this repository.

My impl, C, also uses an extra wordlist for selecting the answer word from, whereas the python version uses one. I felt that some of the words in that wordlist were too obscure for being the final answer, so I only accept those as inputs to give clues from, but the answer will be selected from the smaller, more sensible list.

Oh, we didn't have any restrictions in terms of syntax, functions nor indentation. That's why my C impl is so bloated: and me being lazy lmao sorry. 🤣💀

use:

     gcc/cc wordle.c
     ./a.out
     
     python3 wordle.py

Oh and I also made a helper program for wordle, for those who are interested in cheating. You need to re-open the cut wordlist that it creates every time you give it clues as it shortens the list and updates it. The user interface is not very practical, but alas, it works: I used it to win wordle. 😌

use:

     gcc/cc wordle-helper.c
     ./a.out
