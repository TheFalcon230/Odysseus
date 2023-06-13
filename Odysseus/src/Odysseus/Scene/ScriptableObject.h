#pragma once
#include "Entity.h"

namespace Odysseus
{
	class ScriptableObject
	{
	public:
		virtual ~ScriptableObject() = default;

		template<typename T>
		T& GetComponent()
		{
			return object.GetComponent<T>();
		}
	protected:
		virtual void Start() {}
		virtual void Update(Timestep ts) {}
		virtual void OnDestroy() {}
	private:
		Object object;
		friend class Scene;
	};


}