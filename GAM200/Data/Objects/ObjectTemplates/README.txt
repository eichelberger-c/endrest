Here are all the possible components to add to json files (notes at the bottom of this file):

// animation -- animate is true or false, begin and end are what part of
// sprite sheet you are using (first square has to be 0)
"Animation": { "begin": 0,
			   "end": 79, 
			   "duration": 0.200,
			   "animate": true
            }

// audio -- change filename.wav to any wav file
"Audio": "Data/Audio/filename.wav"


// behavior -- can be SlowChase, FastChase, MimicPlayer, PointToPoint (not yet)
"Behavior": "SlowChase"


// circle collider
"Collider": { "type" : "circle",
              "radius" : 0.0
            }

// square collider (dynamic is true or false)
"Collider": { "type": "square",
			  "width" : 0.0,
              "height" : 0.0, 
              "dynamic" : false
            }

// health -- replace 0 with integer value that will be MAX health
"Health": 0


// physics (needs empty quotes)
"Physics": ""



// sprite -- if not using sprite sheet rows and columns are both 1
//           change filename to what you are using
"Sprite": { "image": "Data/Textures/filename.png",
			"rows": 1,
			"columns": 1
		  }


// transform -- do not change values
"Transform": { "x" : 0.0, 
             "y" : 0.0
             }












Notes:
When adding components to json file, everything within curly braces is considered a json 'object'

i.e. 
{ // main object

  // member 1 -- not an object
  "component1": "blahblah",                         <-- notice that all members
  													    of each object have a comma at the end, except the last member of the object
  // member 2 -- sub object
  "component2": {  
  				   "subshiz":"blahblah",            <-- comma 
  				   "subfloatshiz" : 666.6,          <-- comma
  				   "subintshiz" : 420               <-- no comma
                },                                  <-- comma

  // member 3 -- sub object
  "component3": {
                   "subshiz": "blahblah",           <-- comma 
                   "subshiz2": 666.6,               <-- comma
                   "subshiz3": 420                  <-- no comma
                }                                   <-- no comma
}