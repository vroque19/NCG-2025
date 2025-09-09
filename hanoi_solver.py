import sys

# Initialize poles: pole 1 has all disks, poles 2 and 3 are empty
poles = [
    [6, 5, 4, 3, 2, 1],   # Pole 0 (bottom to top)
    [],                   # Pole 1
    []                    # Pole 2
]

# Track moves made throughout the game
move_history = [(0,0)]
def move(source, destination):
    move_history.append((source, destination))
    # print(f"{source}->{destination}")

def auto_solve_hanoi(n, source, destination):
    if n == 1: # base case
        move(source, destination)
    else:
        aux = 3 - (source + destination)
        auto_solve_hanoi(n - 1, source, aux) # recursive call
        move(source, destination)
        auto_solve_hanoi(n-1, aux, destination) # recursive call


def print_poles():
    for i, pole in enumerate(poles, 1):
        print(f"Pole {i}: {pole}")


def check_legality():
    """Check if all poles are in descending order (largest on bottom)."""
    # Assume legal by default
    is_legal = True

    # Iterate to find violations
    for i, pole in enumerate(poles, 1):
        if any(pole[j] < pole[j+1] for j in range(len(pole)-1)):
            print(f"Illegal: Pole {i} is not in descending order: {pole}")
            is_legal = False
    return is_legal


def query_player():
    """Ask player for a move, or quit / undo.
    
    Undo returns the reverse of the last move recorded to history.
    """
    # Query for input
    user_input = input("\nMove: ").strip()

    # Handle quit / undo
    if user_input.lower() == 'q':
        sys.exit(0)

    if user_input.lower() == 'u':
        last_undo = get_inverse_move(move_history[-1])
        print("--> ", last_undo)
        return last_undo

    # Parse move from user input
    try:
            src_str, dst_str = user_input.split(",")
            src = int(src_str) - 1
            dest = int(dst_str) - 1
    except Exception as e:
        print("Invalid input. Please enter as source_pole,dest_pole (e.g., 1,3).")

    # Make sure src and dest are valid options
    if src not in [0, 1, 2] or dest not in [0, 1, 2]:
        raise Exception("Pole numbers must be 1, 2, or 3.")
    
    return (src, dest)
     

def move_disk(src, dest):
        # Error if source pole doesn't have any disks
        if not poles[src]:
            raise Exception(f"Pole {src} is empty. No disk to move.")

        # Pop the top disk from source and push to destination
        disk = poles[src].pop()
        poles[dest].append(disk)

        # Check if move is a reversal of the last move
        if (src,dest) == get_inverse_move(move_history[-1]):
             # If so, check whether this is the initial state
            if len(move_history) > 1:
                # If not, pop it off the stack.
                move_history.pop()
            else:
                # Do nothing... we're back to the original (0,0). Leave it.
                pass
        else:
            # New move
            move_history.append((src,dest))


def get_inverse_move(move:tuple) -> tuple:
    """Provides the coordinates for undoing a move.

    Parameters
    ----------
    move : tuple
        Move is a tuple in the format (source, destination).

    Returns
    -------
    tuple
        Returns a new (source, destination) which is the reverse of the input.
    """
    last_move = move_history[-1]

    # Flip src/dest to reverse
    src = last_move[1]
    dest = last_move[0]

    return (src, dest)
     

# Run Game
print("Towers of Hanoi \n")
# print("Enter moves as source_pole,dest_pole ('q' to quit, 'u' to undo). \n")

# while True:
#     print_poles()
    
#     src, dest = query_player()
#     move_disk(src, dest)
    
#     check_legality()
print("Optimal Solve for 4 rings:")
auto_solve_hanoi(4, 0, 2)
for i in range(1, len(move_history)):

    print(f"{i}: {move_history[i]}")


