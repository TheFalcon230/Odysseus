#pragma once


namespace Odysseus
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time)
		{
		}
		/// <summary>
		/// Return the time value as a number of seconds.
		/// </summary>
		/// <returns>Time in seconds.</returns>
		float AsSeconds() const { return m_Time; }
		/// <summary>
		/// Return the time value as a number of milliseconds.
		/// </summary>
		/// <returns>Time in milliseconds.</returns>
		float AsMilliseconds() const { return m_Time * 1000.f; }

		operator float() const { return m_Time; }

	private:
		float m_Time = 0.0f;
	};

}
