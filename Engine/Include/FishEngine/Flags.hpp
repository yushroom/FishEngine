//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include <cstdint>

namespace FishEngine
{
	/** @addtogroup General
	 *  @{
	 */

	/** Wrapper around an enum that allows simple use of bitwise logic operations. */
	template<typename Enum, typename Storage = uint32_t>
	class Flags
	{
	public:
		typedef Storage InternalType;

		Flags()
			:mBits(0)
		{ }
					
		Flags(Enum value)
		{
			mBits = static_cast<Storage>(value);
		}

		Flags(const Flags<Enum, Storage>& value)
		{
			mBits = value.mBits;
		}

		explicit Flags(Storage bits)
		{
			mBits = bits;
		}

		/** Checks whether all of the provided bits are set */
		bool isSet(Enum value) const
		{
			return (mBits & static_cast<Storage>(value)) == static_cast<Storage>(value);
		}

		/** Activates all of the provided bits. */
		Flags<Enum, Storage>& set(Enum value)
		{
			mBits = static_cast<Storage>(value);

			return *this;
		}

		/** Deactivates all of the provided bits. */
		void unset(Enum value)
		{
			mBits &= ~static_cast<Storage>(value);
		}

		bool operator==(Enum rhs) const
		{
			return mBits == static_cast<Storage>(rhs);
		}

		bool operator==(const Flags<Enum, Storage>& rhs) const
		{
			return mBits == rhs.mBits;
		}

		bool operator==(bool rhs) const
		{
			return ((bool)*this) == rhs;
		}

		bool operator!=(Enum rhs) const
		{
			return mBits != static_cast<Storage>(rhs);
		}

		bool operator!=(const Flags<Enum, Storage>& rhs) const
		{
			return mBits != rhs.mBits;
		}

		Flags<Enum, Storage>& operator= (Enum rhs)
		{
			mBits = static_cast<Storage>(rhs);

			return *this;
		}

		Flags<Enum, Storage>& operator= (const Flags<Enum, Storage>& rhs)
		{
			mBits = rhs.mBits;

			return *this;
		}

		Flags<Enum, Storage>& operator|= (Enum rhs)
		{
			mBits |= static_cast<Storage>(rhs);

			return *this;
		}

		Flags<Enum, Storage>& operator|= (const Flags<Enum, Storage>& rhs)
		{
			mBits |= rhs.mBits;

			return *this;
		}

		Flags<Enum, Storage> operator| (Enum rhs) const
		{
			Flags<Enum, Storage> out(*this);
			out |= rhs;

			return out;
		}

		Flags<Enum, Storage> operator| (const Flags<Enum, Storage>& rhs) const
		{
			Flags<Enum, Storage> out(*this);
			out |= rhs;

			return out;
		}

		Flags<Enum, Storage>& operator&= (Enum rhs)
		{
			mBits &= static_cast<Storage>(rhs);

			return *this;
		}

		Flags<Enum, Storage>& operator&= (const Flags<Enum, Storage>& rhs)
		{
			mBits &= rhs.mBits;

			return *this;
		}

		Flags<Enum, Storage> operator& (Enum rhs) const
		{
			Flags<Enum, Storage> out = *this;
			out.mBits &= static_cast<Storage>(rhs);

			return out;
		}

		Flags<Enum, Storage> operator& (const Flags<Enum, Storage>& rhs) const
		{
			Flags<Enum, Storage> out = *this;
			out.mBits &= rhs.mBits;

			return out;
		}

		Flags<Enum, Storage>& operator^= (Enum rhs)
		{
			mBits ^= static_cast<Storage>(rhs);

			return *this;
		}

		Flags<Enum, Storage>& operator^= (const Flags<Enum, Storage>& rhs)
		{
			mBits ^= rhs.mBits;

			return *this;
		}

		Flags<Enum, Storage> operator^ (Enum rhs) const
		{
			Flags<Enum, Storage> out = *this;
			out.mBits ^= static_cast<Storage>(rhs);

			return out;
		}

		Flags<Enum, Storage> operator^ (const Flags<Enum, Storage>& rhs) const
		{
			Flags<Enum, Storage> out = *this;
			out.mBits ^= rhs.mBits;

			return out;
		}

		Flags<Enum, Storage> operator~ () const
		{
			Flags<Enum, Storage> out;
			out.mBits = (Storage)~mBits;

			return out;
		}

		operator bool() const
		{
			return mBits ? true : false;
		}

		operator unsigned char() const
		{
			return static_cast<unsigned char>(mBits);
		}

		operator uint16_t() const
		{
			return static_cast<uint16_t>(mBits);
		}

		operator uint32_t() const
		{
			return static_cast<uint32_t>(mBits);
		}

		friend Flags<Enum, Storage> operator&(Enum a, Flags<Enum, Storage> &b)
		{
			Flags<Enum, Storage> out;
			out.mBits = a & b.mBits;
			return out;
		}

	private:
		Storage mBits;
	};

/** Defines global operators for a Flags<Enum, Storage> implementation. */
#define BS_FLAGS_OPERATORS(Enum) BS_FLAGS_OPERATORS_EXT(Enum, uint32_t)

/** Defines global operators for a Flags<Enum, Storage> implementation. */
#define BS_FLAGS_OPERATORS_EXT(Enum, Storage)																   \
		inline Flags<Enum, Storage> operator|(Enum a, Enum b) { Flags<Enum, Storage> r(a); r |= b; return r; } \
		inline Flags<Enum, Storage> operator&(Enum a, Enum b) { Flags<Enum, Storage> r(a); r &= b; return r; } \
		inline Flags<Enum, Storage> operator~(Enum a) { return ~Flags<Enum, Storage>(a); }

	/** @cond SPECIALIZATIONS */
}

/** @cond STDLIB */

namespace std
{
	/** Hash value generator for Flags<Enum, Storage>. */
	template<class Enum, class Storage>
	struct hash<FishEngine::Flags<Enum, Storage>>
	{
		size_t operator()(const FishEngine::Flags<Enum, Storage>& key) const
		{
			return (Storage)key;
		}
	};
}

/** @endcond */