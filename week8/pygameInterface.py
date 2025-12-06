#interface to view simulation using pygame
import pygame
from pyImp import *
import sys

WHITE = (255, 255, 255) #roads
BLACK = (0, 0, 0) #background
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

pygame.init()

WIDTH, HEIGHT = 800, 800
MARGIN = 100#margin around the roads
CELL = (WIDTH - 2 * MARGIN) // (N-1)


screen = pygame.display.set_mode((WIDTH, HEIGHT))
clock = pygame.time.Clock()
t=0
running = True

def toScreen(x, y):
    return MARGIN + x * CELL, MARGIN + y * CELL
    
def drawGrid():
    for i in range(N):
        for j in range(N):
            x, y = toScreen(i, j)

            if j < N - 1:#horizontal road
                nx, ny = toScreen(i, j+1)
                pygame.draw.line(screen, WHITE, (x, y), (nx, ny), 5)
            if i < N - 1:#vertical road
                nx, ny = toScreen(i+1, j)
                pygame.draw.line(screen, WHITE, (x, y), (nx, ny), 5)
    for i in range(N):
        for j in range(N):
            x, y = toScreen(i,j)
            pygame.draw.circle(screen, GREEN, (x, y), 2)#reminder update to traffic light color later 

def drawCars():
    for (src, dst), q in stopped.items():
        for car in q:
            sx, sy = toScreen(x, y)
            pygame.draw.circle(screen, RED, (sx, sy), 5)
        



while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    stepSim(t)
    screen.fill(BLACK)
    drawGrid()
    drawCars()
    pygame.display.flip()
    t+=1
    clock.tick(60)