#pragma once

class Render
{
public:
	auto CreateOverlay() -> bool;
	auto RenderThread() -> void;
};

inline Render Render_;