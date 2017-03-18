#pragma once

#include "Vector2.hpp"
#include "Mathf.hpp"
#include "Vector3.hpp"

namespace FishEngine
{
	class Rect
	{
	private:
		float m_XMin;
		float m_YMin;
		float m_Width;
		float m_Height;

	public:

		Rect(float x, float y, float width, float height)
		{
			this->m_XMin = x;
			this->m_YMin = y;
			this->m_Width = width;
			this->m_Height = height;
		}

		Rect(Vector2 const & position, Vector2 const & size)
		{
			this->m_XMin = position.x;
			this->m_YMin = position.y;
			this->m_Width = size.x;
			this->m_Height = size.y;
		}

		Rect(Rect const & source)
		{
			this->m_XMin = source.m_XMin;
			this->m_YMin = source.m_YMin;
			this->m_Width = source.m_Width;
			this->m_Height = source.m_Height;
		}

		Rect& operator=(Rect const & source)
		{
			this->m_XMin = source.m_XMin;
			this->m_YMin = source.m_YMin;
			this->m_Width = source.m_Width;
			this->m_Height = source.m_Height;
			return *this;
		}

		static Rect MinMaxRect(float xmin, float ymin, float xmax, float ymax)
		{
			return Rect(xmin, ymin, xmax - xmin, ymax - ymin);
		}

		void Set(float x, float y, float width, float height)
		{
			this->m_XMin = x;
			this->m_YMin = y;
			this->m_Width = width;
			this->m_Height = height;
		}

		static Rect Zero;


		float x() const
		{
			return m_XMin;
		}

		void setX(float x)
		{
			m_XMin = x;
		}


		float y() const
		{
			return m_YMin;
		}

		void setY(float y)
		{
			m_YMin = y;
		}


		Vector2 position() const
		{
			return Vector2( m_XMin, m_YMin );
		}

		void setPosition(Vector2 const & value)
		{
			m_XMin = value.x;
			m_YMin = value.y;
		}


		Vector2 center() const
		{
			return Vector2( m_XMin + m_Width*0.5f, m_YMin + m_Height*0.5f );
		}

		void setCenter(Vector2 const & value)
		{
			m_XMin = value.x - m_Width*0.5f;
			m_YMin = value.y - m_Height*0.5f;
		}


		Vector2 min() const
		{
			return Vector2( m_XMin, m_YMin );
		}

		void setMin(Vector2 const & value)
		{
			m_XMin = value.x;
			m_YMin = value.y;
		}


		Vector2 max() const
		{
			return Vector2( xMax(), yMax() );
		}

		void setMax(Vector2 const & value)
		{
			setXMax( value.x );
			setYMax( value.y );
		}


		float width() const
		{
			return m_Width;
		}

		void setWidth(float width)
		{
			m_Width = width;
		}

		float height() const
		{
			return m_Height;
		}

		void setHeight(float height)
		{
			m_Height = height;
		}


		Vector2 size() const
		{
			return Vector2( m_Width, m_Height );
		}

		void setSize(Vector2 const & value)
		{
			m_Width = value.x;
			m_Height = value.y;
		}


		float xMin() const
		{
			return m_XMin;
		}

		void setXMin(float value)
		{
			float xMax = this->xMax();
			this->m_XMin = value;
			this->m_Width = xMax - this->m_XMin;
		}


		float yMin() const
		{
			return m_YMin;
		}

		void setYMin(float value)
		{
			float yMax = this->yMax();
			this->m_YMin = value;
			this->m_Height = yMax - this->m_YMin;
		}


		float xMax() const
		{
			return this->m_Width + this->m_XMin;
		}

		void setXMax(float value)
		{
			this->m_Width = value - this->m_XMin;
		}


		float yMax() const
		{
			return this->m_Height + this->m_YMin;
		}

		void setYMax(float value)
		{
			this->m_Height = value - this->m_YMin;
		}


		bool Contains(Vector2 const & point)
		{
			return point.x >= this->m_XMin && point.x < this->xMax() && point.y >= this->m_YMin && point.y < this->yMax();
		}

		bool Contains(Vector3 const & point)
		{
			return point.x >= this->m_XMin && point.x < this->xMax() && point.y >= this->m_YMin && point.y < this->yMax();
		}

		bool Contains(Vector3 const & point, bool allowInverse)
		{
			bool result;
			if (!allowInverse)
			{
				result = this->Contains(point);
			}
			else
			{
				bool flag = false;
				if ((this->width() < 0.f && point.x <= this->m_XMin && point.x > this->xMax()) || (this->width() >= 0.f && point.x >= this->m_XMin && point.x < this->xMax()))
				{
					flag = true;
				}
				result = (flag && ((this->height() < 0.f && point.y <= this->m_YMin && point.y > this->yMax()) || (this->height() >= 0.f && point.y >= this->m_YMin && point.y < this->yMax())));
			}
			return result;
		}

	private:
		static Rect OrderMinMax(Rect rect)
		{
			if (rect.m_XMin > rect.xMax())
			{
				float xMin = rect.m_XMin;
				rect.m_XMin = rect.xMax();
				rect.setXMax(xMin);
			}
			if (rect.m_YMin > rect.yMax())
			{
				float yMin = rect.m_YMin;
				rect.m_YMin = rect.yMax();
				rect.setYMax(yMin);
			}
			return rect;
		}

	public:

		bool Overlaps(Rect const & other)
		{
			return other.xMax() > this->xMin() && other.xMin() < this->xMax() && other.yMax() > this->yMin() && other.yMin() < this->yMax();
		}

		bool Overlaps(Rect other, bool allowInverse)
		{
			Rect rect = *this;
			if (allowInverse)
			{
				rect = Rect::OrderMinMax(rect);
				other = Rect::OrderMinMax(other);
			}
			return rect.Overlaps(other);
		}

		static Vector2 NormalizedToPoint(Rect const & rectangle, Vector2 const & normalizedRectCoordinates)
		{
			return Vector2(Mathf::Lerp(rectangle.x(), rectangle.xMax(), normalizedRectCoordinates.x), Mathf::Lerp(rectangle.y(), rectangle.yMax(), normalizedRectCoordinates.y));
		}

		static Vector2 PointToNormalized(Rect const & rectangle, Vector2 const & point)
		{
			return Vector2(Mathf::InverseLerp(rectangle.x(), rectangle.xMax(), point.x), Mathf::InverseLerp(rectangle.y(), rectangle.yMax(), point.y));
		}

		friend bool operator !=(Rect const & lhs, Rect const &  rhs)
		{
			return lhs.m_XMin != rhs.m_XMin || lhs.m_YMin != rhs.m_YMin || lhs.m_Width != rhs.m_Width || lhs.m_Height != rhs.m_Height;
		}

		friend bool operator ==(Rect const &  lhs, Rect const &  rhs)
		{
			return lhs.m_XMin == rhs.m_XMin && lhs.m_YMin == rhs.m_YMin && lhs.m_Width == rhs.m_Width && lhs.m_Height == rhs.m_Height;
		}
	};
}
