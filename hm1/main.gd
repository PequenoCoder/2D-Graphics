extends Node

var Mob := preload("res://mob.tscn")

var score = 0
var level_completed = false
var game_started = false

func _ready():
	randomize()

func _on_player_hit():
	game_over()

func game_over():
	$MobTimer.stop()
	$StartTimer.stop()
	$HUD.show_game_over()

func new_game():
	print("Starting new game!")
	get_tree().call_group("mobs", "queue_free")
	$HUD.show_message("Get Ready")
	score = 0
	level_completed = false
	game_started = false
	# Reset StartTimer to 2 seconds for countdown
	$StartTimer.wait_time = 2.0
	$HUD.update_score(score)  # Update HUD with reset score
	print("Score reset to: ", score)
	$Player.start($StartPosition.position)
	$StartTimer.start()

func _on_StartTimer_timeout():
	if not game_started:
		# Initial countdown finished, start the game
		print("Countdown finished, starting game!")
		game_started = true
		$MobTimer.start()
		# Change timer to 1 second intervals for scoring
		$StartTimer.wait_time = 1.0
		$StartTimer.start()


func _on_MobTimer_timeout():
	# Choose a random location on Path2D.
	$MobPath/MobSpawnLocation.progress = randi()
	# Create a Mob instance and add it to the scene.
	var mob = Mob.instantiate()
	add_child(mob)
	# Set the mob's direction perpendicular to the path direction.
	var direction = $MobPath/MobSpawnLocation.rotation + PI / 2
	# Set the mob's position to a random location.
	mob.position = $MobPath/MobSpawnLocation.position
	# Add some randomness to the direction.
	direction += randf_range(-PI / 4, PI / 4)
	mob.rotation = direction
	# Set the velocity (speed & direction).
	mob.linear_velocity = Vector2(randf_range(mob.min_speed, mob.max_speed), 0)
	mob.linear_velocity = mob.linear_velocity.rotated(direction)

func _on_end_zone_body_entered(body):
	print("End zone triggered by: ", body.name)
	if body.name == "Player" and not level_completed:
		print("Level Complete!")
		level_completed = true
		level_complete()

func level_complete():
	$MobTimer.stop()
	$StartTimer.stop()
	if has_node("HUD"):
		$HUD.show_level_complete()
	else:
		print("HUD node not found!")
		print("Level Complete! Congratulations!")


func _on_start_timer_increase() -> void:
	score += 1
	print("Score: ", score)
	$HUD.update_score(score)
	# Restart the timer to keep incrementing
	$StartTimer.start()
