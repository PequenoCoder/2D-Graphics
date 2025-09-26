#ifndef FADESPRITE_H
#define FADESPRITE_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/timer.hpp>

namespace godot {

class FadeSprite : public Sprite2D {
	GDCLASS(FadeSprite, Sprite2D)

private:
	double fade_duration; 
	bool is_fading;
	bool is_visible_state;
	Timer* fade_timer;
	Timer* fade_step_timer;
	double fade_start_alpha;
	double fade_target_alpha;
	double fade_current_alpha;

protected:
	static void _bind_methods();

public:
	FadeSprite();
	~FadeSprite();

	void _ready() override;
	
	void set_fade_duration(const double p_duration);
	double get_fade_duration() const;
	
	void start_fade_out();
	void start_fade_in();
	void reset_visibility();
	bool get_is_fading() const;
	void start_gradual_fade(double from_alpha, double to_alpha);
	
	void _on_fade_completed();
	void _on_timer_timeout();
	void _on_fade_step();
};

}

#endif
