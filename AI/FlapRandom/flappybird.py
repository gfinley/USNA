#! /usr/bin/env python3

"""Flappy Bird, implemented using Pygame."""

import math
import os
from random import randint
from collections import deque
import pygame
import random
from pygame.locals import *
import pickle
import datetime
import time
import math

class dSpace:
    """spaces represent a descritized portion of the birds approach
    Attributes:
    x: x value of the square *** mod with something to get a simple int
    y: y y value of the square *** mod with somethign to get a simeple int
    p: momentem % 33.3 (yeilds a number between 0-9)
    flapQ: q-value for flapping
    nFlapQ: q-value for NOT flapping
    reward: this spaces reward Value// rewards will be assigned later on, atlead negative ones.
    ***Forced exploration should probably be implemented---will do Later
    """
    def __init__(self, x, y,p):
        self.x = x
        self.y = y
        self.p = p
        self.flapQ = 0
        self.noFlapVal = 0
        self.reward = 0
        self.visited = 0


FPS = 60
ANIMATION_SPEED = 0.18  # pixels per millisecond
WIN_WIDTH = 284 * 2     # BG image size: 284x512 px; tiled twice
WIN_HEIGHT = 512
gamma = .8
k = 10
pp = True
randomV = input("please enter randomness Value")
vmapName = "vMapR" + str(randomV)
rmapName = "rewardMapR" + str(randomV)
save = "saveR" + str(randomV)
#global fail
#global success 
#oldPercent = 0
#counter = 0
try:
    visitedMap = pickle.load( open( vmapName, "rb" ) )
except:
        visitedMap = {}

try:
    rewardMap  = pickle.load( open( rmapName, "rb" ) )
except:
        rewardMap = {}
momentem_size = 333.3
x_size = 32
y_size = 32
try:
    dSpaceArray = pickle.load( open( save, "rb" ) )
except:
    dSpaceArray = [[[0 for _ in range(int(momentem_size/(33.3)))] for _ in range(2*int(WIN_HEIGHT/ y_size))] for _ in range(0, int(WIN_WIDTH/ x_size))]
    for x in range(0, int(WIN_WIDTH/ x_size)):
        for y in range(0, 2 * int(WIN_HEIGHT/ y_size)):
            for z in range(0, int(momentem_size/(33.3))):
            #print(x,y,z)
                dSpaceArray[x][y][z] = dSpace(x,y,z)

"""in the main function it will go
    catchFail // assign negative reward
    catchpass // assign positive reward
    stateChange//
        when it changes state
        -updateQ Values
        -Pick another move
"""





class stateActionState:
    def __init__(self, state, action,state2):
        self.originalStatex = state.x
        self.originalStatey = state.y
        self.originalStatep = state.p
        self.action = action
        self.finalStatex = state2.x
        self.finalStatey = state2.y
        self.finalStatep = state2.p

#reward map 

def getQ(state,action):
    """ Attributes:
    state: current state
    action: action withing state 1 = flap, 0 = NO flap
    """
    if action == 0:
        return state.noFlapVal
    if action == 1:
        return state.flapQ

def changeQ(val, state,action):
    if action == 1 :
        state.flapQ = val
    if action == 0:
        state.noFlapVal = val

def getMaxQ(state):
    flapVal = getQ(state, 1)
    noFlapVal = getQ(state, 0)
    if(flapVal >= noFlapVal):
        return flapVal
    else:
        return noFlapVal
def printState(state):
    print(state.x,state.y,state.p)


def getReward(state, action, state2):
    if (state.x,state.y,state.p,action,state2.x,state2.y,state2.p) in rewardMap:
        temp = rewardMap[(state.x,state.y,state.p,action,state2.x,state2.y,state2.p)]
        return temp
    return 0

def getNextAction(state):
    flapVal = getQ(state, 1)
    noFlapVal = getQ(state, 0)
    ra = random.randrange(0,1000)
    if ra > int(randomV):
        if(flapVal == noFlapVal):
            return 0
            #return random.randrange(0,2)
        if(flapVal > noFlapVal):
            return 1
        else:
            return 0
    else:
        return random.choice([True, False])

def getAlpha(state):
    if (state.x,state.y,state.p) in visitedMap:
        #print("visited state again")
        visitedMap[(state.x,state.y,state.p)] = visitedMap[(state.x,state.y,state.p)] + 1
        #print(1/math.log(10 + visitedMap[(state.x,state.y,state.p)]))
        return 1/math.log(9 + visitedMap[(state.x,state.y,state.p)])
    else:
        visitedMap[(state.x,state.y,state.p)] = 1
        #print("added state to map")
        return 1

def getSample(state, action, state2):
    temp = getReward(state,action,state2) + (gamma * getMaxQ(state2))    
    #if temp < -10:
    #print(getReward(state,action,state2))
    #print("getSample" + str(getReward(state , action,state2)) + " " + str((gamma * maxExploreFunction(state2)))) 
    #print("getSample Value is " + str(temp))   
    return temp

def updateQ(state,  action, state2):
    alpha = getAlpha(state)
    temp = (1-alpha) * getQ(state,action) + alpha * getSample(state,action,state2)
    #print((1-alpha),getQ(state,action), alpha, getSample(state,action,state2))
    #print((1-alpha),getQ(state,action), alpha, getSample(state,action,state2))
    if temp != 0:
        #print((1-alpha),getQ(state,action), alpha, getSample(state,action,state2))
        #print(temp)
        changeQ(temp,state,action)

def maxExploreFunction(state):
    if (state.x,state.y,state.p) in visitedMap:
        temp = visitedMap[(state.x,state.y,state.p)]
    else:
        temp = 1
    val1 = exploreFunction(getQ(state,0), temp)
    val2 = exploreFunction(getQ(state,1), temp)
    #print(" Val1: " + str(val1))
    #print(" Val2: " + str(val2))
    if val1 > val2:
        return val1
    else:
        return val2

def exploreFunction(u,n):
    return u + k/n

def createSuperArray(x_size, y_size, momentem_size):
    count = 0
    try:
        save = "saveR" + str(random)
        cat = pickle.load( open( save, "rb" ) )
    except:
        save = "saveR" + str(random)
        cat = [[[0 for _ in range(int(momentem_size/(33.3)))] for _ in range(2*int(WIN_HEIGHT/ y_size))] for _ in range(0, int(WIN_WIDTH/ x_size))]
        for x in range(0, int(WIN_WIDTH/ x_size)):
            for y in range(0, 2 * int(WIN_HEIGHT/ y_size)):
              for z in range(0, int(momentem_size/(33.3))):
                #print(x,y,z)
                cat[x][y][z] = dSpace(x,y,z)
    #cat = [[[0 for _ in range(int(WIN_WIDTH/ x_size))] for _ in range(int(WIN_HEIGHT/ y_size))] for _ in range(int(momentem_size/33.3))]
    
    return cat 

def catchDone(state,action,state2):
    if (state.x,state.y,state.p,action,state2.x,state2.y,state2.p) not in rewardMap:
        rewardMap[(state.x,state.y,state.p,action,state2.x,state2.y,state2.p)] = -100
    print("punished")


def catchScore(state,action,state2):
    rewardMap[(state.x,state.y,state.p,action,state2.x,state2.y,state2.p)] = 1000
    print("MADE IT THROUGH!!!!!!!!!!!!!!!!!!!!")
    #success = success + 1
    #counter = counter + 1

def tX(x):
    temp = int(x/32)
    if temp > 16:
        return 16
    if temp < 0:
        return 0;
    return temp
def tY(y):
    return 15 + int(y/32)
def tP(p):
    return int(p/(33.4))

class Bird(pygame.sprite.Sprite):
    """Represents the bird controlled by the player.

    The bird is the 'hero' of this game.  The player can make it climb
    (ascend quickly), otherwise it sinks (descends more slowly).  It must
    pass through the space in between pipes (for every pipe passed, one
    point is scored); if it crashes into a pipe, the game ends.

    Attributes:
    x: The bird's X coordinate.
    y: The bird's Y coordinate.
    msec_to_climb: The number of milliseconds left to climb, where a
        complete climb lasts Bird.CLIMB_DURATION milliseconds.

    Constants:
    WIDTH: The width, in pixels, of the bird's image.
    HEIGHT: The height, in pixels, of the bird's image.
    SINK_SPEED: With which speed, in pixels per millisecond, the bird
        descends in one second while not climbing.
    CLIMB_SPEED: With which speed, in pixels per millisecond, the bird
        ascends in one second while climbing, on average.  See also the
        Bird.update docstring.
    CLIMB_DURATION: The number of milliseconds it takes the bird to
        execute a complete climb.
    """

    WIDTH = HEIGHT = 32
    SINK_SPEED = 0.18
    CLIMB_SPEED = 0.3
    CLIMB_DURATION = 333.3

    def __init__(self, x, y, msec_to_climb, images):
        """Initialise a new Bird instance.

        Arguments:
        x: The bird's initial X coordinate.
        y: The bird's initial Y coordinate.
        msec_to_climb: The number of milliseconds left to climb, where a
            complete climb lasts Bird.CLIMB_DURATION milliseconds.  Use
            this if you want the bird to make a (small?) climb at the
            very beginning of the game.
        images: A tuple containing the images used by this bird.  It
            must contain the following images, in the following order:
                0. image of the bird with its wing pointing upward
                1. image of the bird with its wing pointing downward
        """
        super(Bird, self).__init__()
        self.x, self.y = x, y
        self.msec_to_climb = msec_to_climb
        self._img_wingup, self._img_wingdown = images
        self._mask_wingup = pygame.mask.from_surface(self._img_wingup)
        self._mask_wingdown = pygame.mask.from_surface(self._img_wingdown)

    def update(self, delta_frames=1):
        """Update the bird's position.

        This function uses the cosine function to achieve a smooth climb:
        In the first and last few frames, the bird climbs very little, in the
        middle of the climb, it climbs a lot.
        One complete climb lasts CLIMB_DURATION milliseconds, during which
        the bird ascends with an average speed of CLIMB_SPEED px/ms.
        This Bird's msec_to_climb attribute will automatically be
        decreased accordingly if it was > 0 when this method was called.

        Arguments:
        delta_frames: The number of frames elapsed since this method was
            last called.
        """
        if self.msec_to_climb > 0:
            frac_climb_done = 1 - self.msec_to_climb/Bird.CLIMB_DURATION
            self.y -= (Bird.CLIMB_SPEED * frames_to_msec(delta_frames) *
                       (1 - math.cos(frac_climb_done * math.pi)))
            self.msec_to_climb -= frames_to_msec(delta_frames)
        else:
            self.y += Bird.SINK_SPEED * frames_to_msec(delta_frames)

    @property
    def image(self):
        """Get a Surface containing this bird's image.

        This will decide whether to return an image where the bird's
        visible wing is pointing upward or where it is pointing downward
        based on pygame.time.get_ticks().  This will animate the flapping
        bird, even though pygame doesn't support animated GIFs.
        """
        if pygame.time.get_ticks() % 500 >= 250:
            return self._img_wingup
        else:
            return self._img_wingdown

    @property
    def mask(self):
        """Get a bitmask for use in collision detection.

        The bitmask excludes all pixels in self.image with a
        transparency greater than 127."""
        if pygame.time.get_ticks() % 500 >= 250:
            return self._mask_wingup
        else:
            return self._mask_wingdown

    @property
    def rect(self):
        """Get the bird's position, width, and height, as a pygame.Rect."""
        return Rect(self.x, self.y, Bird.WIDTH, Bird.HEIGHT)
class PipePair(pygame.sprite.Sprite):
    """Represents an obstacle.

    A PipePair has a top and a bottom pipe, and only between them can
    the bird pass -- if it collides with either part, the game is over.

    Attributes:
    x: The PipePair's X position.  This is a float, to make movement
        smoother.  Note that there is no y attribute, as it will only
        ever be 0.
    image: A pygame.Surface which can be blitted to the display surface
        to display the PipePair.
    mask: A bitmask which excludes all pixels in self.image with a
        transparency greater than 127.  This can be used for collision
        detection.
    top_pieces: The number of pieces, including the end piece, in the
        top pipe.
    bottom_pieces: The number of pieces, including the end piece, in
        the bottom pipe.

    Constants:
    WIDTH: The width, in pixels, of a pipe piece.  Because a pipe is
        only one piece wide, this is also the width of a PipePair's
        image.
    PIECE_HEIGHT: The height, in pixels, of a pipe piece.
    ADD_INTERVAL: The interval, in milliseconds, in between adding new
        pipes.
    """

    WIDTH = 80
    PIECE_HEIGHT = 32
    ADD_INTERVAL = 3000

    def __init__(self, pipe_end_img, pipe_body_img):
        """Initialises a new random PipePair.

        The new PipePair will automatically be assigned an x attribute of
        float(WIN_WIDTH - 1).

        Arguments:
        pipe_end_img: The image to use to represent a pipe's end piece.
        pipe_body_img: The image to use to represent one horizontal slice
            of a pipe's body.
        """
        self.x = float(WIN_WIDTH - 1)
        self.score_counted = False

        self.image = pygame.Surface((PipePair.WIDTH, WIN_HEIGHT), SRCALPHA)
        self.image.convert()   # speeds up blitting
        self.image.fill((0, 0, 0, 0))
        total_pipe_body_pieces = int(
            (WIN_HEIGHT -                  # fill window from top to bottom
             3 * Bird.HEIGHT -             # make room for bird to fit through
             3 * PipePair.PIECE_HEIGHT) /  # 2 end pieces + 1 body piece
            PipePair.PIECE_HEIGHT          # to get number of pipe pieces
        )
        self.bottom_pieces = randint(1, total_pipe_body_pieces)
        self.top_pieces = total_pipe_body_pieces - self.bottom_pieces

        # bottom pipe
        for i in range(1, self.bottom_pieces + 1):
            piece_pos = (0, WIN_HEIGHT - i*PipePair.PIECE_HEIGHT)
            self.image.blit(pipe_body_img, piece_pos)
        bottom_pipe_end_y = WIN_HEIGHT - self.bottom_height_px
        bottom_end_piece_pos = (0, bottom_pipe_end_y - PipePair.PIECE_HEIGHT)
        self.image.blit(pipe_end_img, bottom_end_piece_pos)

        # top pipe
        for i in range(self.top_pieces):
            self.image.blit(pipe_body_img, (0, i * PipePair.PIECE_HEIGHT))
        top_pipe_end_y = self.top_height_px
        self.image.blit(pipe_end_img, (0, top_pipe_end_y))

        # compensate for added end pieces
        self.top_pieces += 1
        self.bottom_pieces += 1

        # for collision detection
        self.mask = pygame.mask.from_surface(self.image)

    @property
    def top_height_px(self):
        """Get the top pipe's height, in pixels."""
        return self.top_pieces * PipePair.PIECE_HEIGHT

    @property
    def bottom_height_px(self):
        """Get the bottom pipe's height, in pixels."""
        return self.bottom_pieces * PipePair.PIECE_HEIGHT

    @property
    def visible(self):
        """Get whether this PipePair on screen, visible to the player."""
        return -PipePair.WIDTH < self.x < WIN_WIDTH

    @property
    def rect(self):
        """Get the Rect which contains this PipePair."""
        return Rect(self.x, 0, PipePair.WIDTH, PipePair.PIECE_HEIGHT)

    def update(self, delta_frames=1):
        """Update the PipePair's position.

        Arguments:
        delta_frames: The number of frames elapsed since this method was
            last called.
        """
        self.x -= ANIMATION_SPEED * frames_to_msec(delta_frames)

    def collides_with(self, bird):
        """Get whether the bird collides with a pipe in this PipePair.

        Arguments:
        bird: The Bird which should be tested for collision with this
            PipePair.
        """
        return pygame.sprite.collide_mask(self, bird)
def load_images():
    """Load all images required by the game and return a dict of them.

    The returned dict has the following keys:
    background: The game's background image.
    bird-wingup: An image of the bird with its wing pointing upward.
        Use this and bird-wingdown to create a flapping bird.
    bird-wingdown: An image of the bird with its wing pointing downward.
        Use this and bird-wingup to create a flapping bird.
    pipe-end: An image of a pipe's end piece (the slightly wider bit).
        Use this and pipe-body to make pipes.
    pipe-body: An image of a slice of a pipe's body.  Use this and
        pipe-body to make pipes.
    """

    def load_image(img_file_name):
        """Return the loaded pygame image with the specified file name.

        This function looks for images in the game's images folder
        (./images/).  All images are converted before being returned to
        speed up blitting.

        Arguments:
        img_file_name: The file name (including its extension, e.g.
            '.png') of the required image, without a file path.
        """
        file_name = os.path.join('.', 'images', img_file_name)
        img = pygame.image.load(file_name)
        img.convert()
        return img

    return {'background': load_image('background.png'),
            'pipe-end': load_image('pipe_end.png'),
            'pipe-body': load_image('pipe_body.png'),
            # images for animating the flapping bird -- animated GIFs are
            # not supported in pygame
            'bird-wingup': load_image('bird_wing_up.png'),
            'bird-wingdown': load_image('bird_wing_down.png')}
def frames_to_msec(frames, fps=FPS):
    """Convert frames to milliseconds at the specified framerate.

    Arguments:
    frames: How many frames to convert to milliseconds.
    fps: The framerate to use for conversion.  Default: FPS.
    """
    return 3000.0 * frames / fps
def msec_to_frames(milliseconds, fps=FPS):
    """Convert milliseconds to frames at the specified framerate.

    Arguments:
    milliseconds: How many milliseconds to convert to frames.
    fps: The framerate to use for conversion.  Default: FPS.
    """
    return fps * milliseconds / 3000.0
 

def main():
    """The application's entry point.

    If someone executes this module (instead of importing it, for
    example), this function is called.
    """
    """ my vars"""
    done2 = False
    highScore = 1
    pygame.init()
    oldState = dSpaceArray[0][0][0]
    counter = 0
    oldPercent = 0
    fail = 0
    success = 0
    counter = 0
    try:
        while not done2:
            #print(rewardMap)
            display_surface = pygame.display.set_mode((WIN_WIDTH, WIN_HEIGHT))
            pygame.display.set_caption('Pygame Flappy Bird')
            


            clock = pygame.time.Clock()
            score_font = pygame.font.SysFont(None, 32, bold=True)  # default font
            distance_font = pygame.font.SysFont(None, 32, bold=True)  # default font
            images = load_images()

            # the bird stays in the same x position, so bird.x is a constant
            # center bird on screen
            bird = Bird(50, int(WIN_HEIGHT/2 - Bird.HEIGHT/2), 2,
                        (images['bird-wingup'], images['bird-wingdown']))

            pipes = deque()

            frame_clock = 0  # this counter is only incremented if the game isn't paused
            score = 0
            distance = 0
            counter = counter + 1
            print(counter)
            if counter > 100:
                pickle.dump( dSpaceArray, open( save, "wb" ) )
                pickle.dump( visitedMap, open( vmapName, "wb" ) )
                pickle.dump( rewardMap, open( rmapName, "wb" ) )
                print("-----------------------------pickle---------------------------")
                counter = 1
            done = paused = False
            #functionality for status updates
            """try:
                if counter % 10 == 0:
                    print("this last 100 runs saw " + str(success) + " sucesses and " + str(fail))
                    print("accuracy is " + str(success/fail))
                    print("this is an improvment of " + str(success/fail - oldPercent))
                    print("----------------------------------")
                    oldPercent = success/fail
                    success = 0
                    failes = 0
                    counter = 0
            except (RuntimeError, TypeError, NameError, ZeroDivisionError) as e:
                print(e)
                pass
            """
            while  not done:
                clock.tick(FPS)
                # Handle this 'manually'.  If we used pygame.time.set_timer(),
                # pipe addition would be messed up when paused.
                if not (paused or frame_clock % msec_to_frames(PipePair.ADD_INTERVAL)):
                    pp = PipePair(images['pipe-end'], images['pipe-body'])
                    pipes.append(pp)

                for e in pygame.event.get():
                    if e.type == QUIT or (e.type == KEYUP and e.key == K_ESCAPE):
                        done2 = done = True 
                        break
                    elif e.type == KEYUP and e.key in (K_PAUSE, K_p):
                        paused = not paused
                    elif e.type == MOUSEBUTTONUP or (e.type == KEYUP and
                            e.key in (K_UP, K_RETURN, K_SPACE)):
                        bird.msec_to_climb = Bird.CLIMB_DURATION

                if paused:
                    paused = not paused  # don't draw anything

                """my vars"""
                currX = pipes[-1].x - bird.x + 100
                currY = ( WIN_HEIGHT - (pipes[-1].bottom_pieces + 1 ) * 32 ) - bird.y - 60
                currP = bird.msec_to_climb
                currState = dSpaceArray[tX(currX)][tY(currY)][tP(currP)]
                climbing= 0
                if tP(currP) > 0:
                    climbing = 1



                # check for collisions
                pipe_collision = any(p.collides_with(bird) for p in pipes)
                if pipe_collision or 0 >= bird.y or bird.y >= WIN_HEIGHT - Bird.HEIGHT:
                    catchDone( oldState, climbing , currState)
                    updateQ(oldState, climbing ,currState)
                    done = True
                if (done == True):
                    distance = pipes[-1].x - bird.x + 100
                    fail = fail + 1



                for x in (0, WIN_WIDTH / 2):
                    display_surface.blit(images['background'], (x, 0))

                while pipes and not pipes[0].visible:
                    pipes.popleft()

                for p in pipes:
                    p.update()
                    display_surface.blit(p.image, p.rect)

                bird.update()
                display_surface.blit(bird.image, bird.rect)

                # update and display score
                for p in pipes:
                    if p.x + PipePair.WIDTH < bird.x and not p.score_counted:
                        score += 1
                        p.score_counted = True
                        catchScore(oldState,climbing,currState)
                        success = success + 1
                        counter = counter + 1
                        




                """updating the Q function here"""
                if oldState.x != currState.x or oldState.y != currState.y or oldState.p != currState.p:
                    updateQ(oldState, climbing ,currState)
                    nextMove = getNextAction(currState)
                    #print("stateChange")
                    #print(oldState.x,oldState.y,oldState.p)
                    #print(currState.x,currState.y,currState.p)
                    if nextMove == 1 and bird.msec_to_climb < 1 :
                        bird.msec_to_climb = Bird.CLIMB_DURATION
                        nextMove = 0
                    stateX = tX(currX)
                    stateY = tY(currY)
                    stateP = tP(currP)
                    oldState = currState



                score_surface = score_font.render(str(score), True, (255, 255, 255))
                score_x = WIN_WIDTH/2 - score_surface.get_width()/2
                display_surface.blit(score_surface, (score_x, PipePair.PIECE_HEIGHT))
                """
                #display distance on game board
                distance = pipes[-1].x - bird.x + 100
                distance_surface = score_font.render(str(distance), True, (255, 255, 255))
                score_x = WIN_WIDTH/2 - score_surface.get_width()/2
                display_surface.blit(distance_surface, (score_x, PipePair.PIECE_HEIGHT-20))

                #y_distance = bird.y - ( WIN_HEIGHT - pipes[-1].bottom_height_px)
                y_distance_to_middle =  ( WIN_HEIGHT - (pipes[-1].bottom_pieces + 1 ) * 32 ) - bird.y - 60
                distance_surface = score_font.render(str(y_distance_to_middle), True, (255, 255, 255))
                score_x = WIN_WIDTH/2 - score_surface.get_width()/2
                display_surface.blit(distance_surface, (score_x, PipePair.PIECE_HEIGHT+30))


    """
                timeToClimb =  bird.msec_to_climb 
                distance_surface = score_font.render(str( timeToClimb), True, (255, 255, 255))
                score_x = WIN_WIDTH/2 - score_surface.get_width()/2
                display_surface.blit(distance_surface, (score_x, PipePair.PIECE_HEIGHT+60))
                




                pygame.display.flip()
                frame_clock += 1
           # print(WIN_HEIGHT)
                #print('Game over! Score: %i' % score)
                if score >= highScore:
                    highScore = score
                    pygame.image.save(display_surface,"highscores/" + str(save) + "-" + str(score) + ".jpg")
        

        pygame.quit()
        pickle.dump( dSpaceArray, open( save, "wb" ) )
        pickle.dump( visitedMap, open( vmapName, "wb" ) )
        pickle.dump( rewardMap, open( rmapName, "wb" ) )
    except:
        pass

if __name__ == '__main__':
    # If this module had been imported, __name__ would be 'flappybird'.
    # It was executed (e.g. by double-clicking the file), so call main.
    main()
