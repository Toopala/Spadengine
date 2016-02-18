#pragma once

namespace sge
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		void update(float step);
		void interpolate(float alpha);
		void draw();
		void push();
		void pop();
		void change();
		void handleScenes();
	private:
		//std::vector<Scene*> scenes;
	};
};