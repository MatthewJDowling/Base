#pragma once

#include "Transform.h"


namespace base
{
	class Collider
	{		
		hull m_hull;
		aabb m_localBox;
		bool m_trigger;

	public:
		Collider(float radius = 0.5f, bool blocking = true)			   : m_hull(radius), m_trigger(blocking) { m_localBox = m_hull.boundingBox(); }
		Collider(const vec2 *ccw, size_t a_size, bool blocking = true) : m_hull(ccw,a_size), m_trigger(blocking) { m_localBox = m_hull.boundingBox(); }
		Collider(const hull &a_hull, bool blocking = true)             : m_hull(a_hull), m_trigger(blocking) { m_localBox = m_hull.boundingBox(); }

		const aabb &getLocalBounds() const { return m_localBox; }	
		const aabb &getGlobalBounds(const Transform *T) const { return T->getGlobalTransform() * m_localBox; }

		const hull &getLocalHull() const { return m_hull; }
		const hull &getGlobalHull(const Transform *T) const { return T->getGlobalTransform() * m_hull; }

		void draw(const Transform *T, const mat3 &C) const
		{
			auto t_aabb = C * T->getGlobalTransform() * m_localBox;

			auto t_hull = C * T->getGlobalTransform() * m_hull;

			sfw::drawLine(t_aabb.min().x, t_aabb.min().y, t_aabb.min().x, t_aabb.max().y, 0x88888888);
			sfw::drawLine(t_aabb.min().x, t_aabb.min().y, t_aabb.max().x, t_aabb.min().y, 0x88888888);
			sfw::drawLine(t_aabb.max().x, t_aabb.max().y, t_aabb.min().x, t_aabb.max().y, 0x88888888);
			sfw::drawLine(t_aabb.max().x, t_aabb.max().y, t_aabb.max().x, t_aabb.min().y, 0x88888888);

			for(int i = 0; i < m_hull.size; ++i)
			{
			sfw::drawLine(t_hull.points[i].x, t_hull.points[i].y, t_hull.points[(i + 1) % m_hull.size].x, t_hull.points[(i+1)%m_hull.size].y, 0xAAAAAAAA);
			}
		}

		bool isTrigger() { return m_trigger; }
		void setTrigger(bool trigger) { m_trigger = trigger; }
	};

	bool BoundsTest(const Transform *AT, const Collider *AC,
					const Transform *BT, const Collider *BC)
	{
		auto A = AC->getGlobalBounds(AT);
		auto B = BC->getGlobalBounds(BT);

		return overlap(A, B);
	}


	collision ColliderTest(const Transform *AT, const Collider *AC,
						   const Transform *BT, const Collider *BC)
	{
		auto A = AC->getGlobalHull(AT);
		auto B = BC->getGlobalHull(BT);

		return collides(A, B);
	}
}