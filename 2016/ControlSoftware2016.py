import pygame
from time import sleep
    
import socket, struct
pygame.init()

#Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Initialize the joysticks
pygame.joystick.init()
    

# -------- Main Program Loop -----------
while done==False:
    # EVENT PROCESSING STEP
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop
        
        # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
        if event.type == pygame.JOYBUTTONDOWN:
            print("Joystick button pressed.")
        if event.type == pygame.JOYBUTTONUP:
            print("Joystick button released.")

    # Get count of joysticks
    joystick_count = pygame.joystick.get_count()

    #print("Number of joysticks: {}".format(joystick_count) )

    
    # For each joystick:
    x=0
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()
    
       # print("Joystick {}".format(i) )
        
    
        # Get the name from the OS for the controller/joystick
        name = joystick.get_name()
       # print("Joystick name: {}".format(name) )
        
        # Usually axis run in pairs, up/down for one, and left/right for
        # the other.
        axes = joystick.get_numaxes()
        #print("Number of axes: {}".format(axes) )
        
        
        for i in range( axes ):
            axis = joystick.get_axis( i )
            #print("Axis {} value: {:>6.3f}".format(i, axis) )
    
            
        buttons = joystick.get_numbuttons()
       # print( "Number of buttons: {}".format(buttons) )
       

        for i in range( buttons ):
            button = joystick.get_button( i )
            #print("Button {:>2} value: {}".format(i,button) )
        
            
        # Hat switch. All or nothing for direction, not like joysticks.
        # Value comes back in an array.
        hats = joystick.get_numhats()
        #print("Number of hats: {}".format(hats) )
    

        for i in range( hats ):
            hat = joystick.get_hat( i )
           # print("Hat {} value: {}".format(i, str(hat)) )
       
        #sleep(.1)
        button6= joystick.get_button(6)
        button7= joystick.get_button(7)
        
        
        track=0
        
        if button6 == 1:
            track = -1
            
        if button7 == 1:
            track = 1
        
        track = track * 127.5
        track = track + 127.5
        

        axis0 = joystick.get_axis( 0 )
        axis1 = joystick.get_axis( 1 )
        axis2 = joystick.get_axis( 2 )
        axis3 = joystick.get_axis( 3 )


        if  .10 > axis0 > -.10:
            axis0=0
        if  .10 > axis1 > -.10:
            axis1=0
        if  .10 > axis2 > -.10:
              axis2=0
        if  .10 > axis3 > -.10:
            axis3=0


        axis0=axis0*127.5 #makes the range (-127.5,127.5)
        axis0=axis0+127.5 #Shifts the center to 127.5. Our range is now (0,255)
    
        axis1=axis1*127.5
        axis1=axis1+127.5
   
        axis2=axis2*127.5
        axis2=axis2+127.5
   
        axis3=axis3*127.5
        axis3=axis3+127.5

        test = axis0+axis1+axis2+axis3+track
        #print test

        
        
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('192.168.1.42', 5050))    #('Ip address', port)  we're using port 5050 it could be any port really but I chose this one


        packer = struct.Struct('B B B B B')
        data = packer.pack(axis2,axis3,axis0,axis1,track)     #stores the values of axis2 into the first bit, and axis3 in the second bit
        if test != 637.5:
            s.send(data)        
        sleep(.1)                        

        
    
    # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT
    
    # Go ahead and update the screen with what we've drawn.


    # Limit to 20 frames per second
    clock.tick(20)
    
# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit ()
