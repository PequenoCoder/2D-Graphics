# GDExtension FadeSprite Implementation - Assignment 2

## Overview

For this assignment, I had to create a GDExtension that extends Godot's Sprite2D class with new functionality. I decided to implement a fade-out effect that triggers when the character is touched, which seemed like a cool feature for my game.

### Key Components
1. Collision Detection: Uses Area2D with CollisionShape2D for touch detection
2. Fade Animation: Custom timer-based system that gradually changes transparency
3. Signal Handling: Proper signal connections for communication with other nodes
4. Editor Integration: FadeSrite appears in menu

## How to Use

1. FadeSprite is in Godot's menu
2. Configure parameters in the Inspector (fade_duration, fade_on_touch)
3. Connect to signals if you want to respond to fade events
4. The player automatically fades out when hit by enemies

## Files Created/Modified

- src/fadesprite.h - Header file for FadeSprite class
- src/fadesprite.cpp - Implementation of FadeSprite class
- src/register_types.cpp - Updated to register FadeSprite
- player.gd - Enhanced with fade functionality
