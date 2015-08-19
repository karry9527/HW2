#include <vector>

class vec
{
	class vec_3
	{
	public:
		float ptr[3];
		vec_3 (float *v) 
		{
			for (size_t i=0;i<3;i++)
				ptr[i] = v[i];
		}
		inline float& operator[](size_t index)
		{
			return ptr[index];
		}
	};
	class vec_2
	{
	public:
		float ptr[2];
		vec_2 (float *v) 
		{
			for (size_t i=0;i<2;i++)
				ptr[i] = v[i];
		}
		inline float& operator[](size_t index)
		{
			return ptr[index];
		}
	};
}