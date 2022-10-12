README

  Team: WHAT
  Project Topic: Movie Review Social Network
  
  Team member:
  	Pechdanai Saepong (Nine) 63070503434 
  	Fasai Sae-Tae (Atom) 63070503436 
   	Chanon Khanijoh (Sean) 63070503408 
  	Pokpong jitjomnongmate (Paul) 63070503425  
  
  Code Running Guide:
	1. change terminal directory to where you put the files in
    2. make
    3. ./main
  *if you can't make and run the code please 'make clean' first*

  Movie Network System Guide:
    1. Register if you haven't had an account yet.
    2. Login to go it to the system / login after finish registered.
    3. Go into the main menu.
      3.1 Write the review: write a new review
        - this feature allows the user to write a new movie review including movie name, genre, rating, and review.
      3.2 Edit your review
        - this feature allows the user to edit their review by input the movie name. Users can edit genre, rating, and review.
				- can only edit review of the account that is login
        - and user can delete that review 
      3.3 Suggestion
        - this feature will suggest a username that the users will likely follow
				- this is base on:
					- who the user liked
					- user with the same genre
					- user that liked the same user as you				
      3.4 Search
        - this feature will allow users to search the review by movie name, username, date, and genre. And after search, the review user can like the review.
      3.5 Display following user's review
        - this feature will allow the user to see their following username
				- user then can see a review of one of the usernames they followed
				- they can also like the review they search
      3.6 Display new feed
        - this feature will show the feed of the new review.
				- this is based on the last login of the user
      3.7 Log out
				- will update the last login date of the user to today's date
      3.8 Exit the program
				- will rewrite data in data structure into a text file where it can be read again next time the program is open