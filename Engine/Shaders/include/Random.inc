#ifndef Random_inc
#define Random_inc

/*
 * Pseudo random number generator, based on "TEA, a tiny Encrytion Algorithm"
 * http://citeseer.ist.psu.edu/viewdoc/download?doi=10.1.1.45.281&rep=rep1&type=pdf
 * http://www.umbc.edu/~olano/papers/index.html#GPUTEA
 * @param v - old seed (full 32bit range)
 * @param IterationCount - >=1, bigger numbers cost more performance but improve quality
 * @return new seed
 */
uint2 ScrambleTEA(uint2 v, uint IterationCount)
{
	// Start with some random data (numbers can be arbitrary but those have been used by others and seem to work well)
	uint k[4] = uint[4]( 0xA341316Cu , 0xC8013EA4u , 0xAD90777Du , 0x7E95761Eu );
	
	uint y = v[0];
	uint z = v[1];
	uint sum = 0;
	
	for(uint i = 0; i < IterationCount; ++i)
	{
		sum += 0x9e3779b9;
		y += ((z << 4u) + k[0]) ^ (z + sum) ^ ((z >> 5u) + k[1]);
		z += ((y << 4u) + k[2]) ^ (y + sum) ^ ((y >> 5u) + k[3]);
	}

	return uint2(y, z);
}

uint2 ScrambleTEA(uint2 v)
{
	return ScrambleTEA(v, 3);
}

#endif
