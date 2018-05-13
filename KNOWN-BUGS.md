# Severity explained:

level 1: Balance issues or minor visual issues.

level 2: A nuisance for the user, either from a technical perspective or mechanical one.
         These bugs are a result of big semantic mistakes in the code.

level 3: Game breaking in nature. Will probably have the same origin as a level 2 bug.


###### Bug template:
- Description: *Mandatory
- Possible solution: *Optional
- Cause: *Optional *guessing is allowed


## LEVEL 3:

## LEVEL 2:
- Description: Going from camera::state::LOCK_TO_TARGET to any other state
				makes camera go back to previous rotation
- Possible solution:
- Cause: pitch and yaw stays the same while in camera::state::LOCK_TO_TARGET

## LEVEL 1:
