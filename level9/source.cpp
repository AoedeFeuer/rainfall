#include <unistd.h>
#include <cstring>

class N {
public:
	int num;
	char annotation[100];

	N(int n)
	{
		this->num = n;
	}

	int operator+(N &op)
	{
		return this->num + op.num;
	}

	int operator-(N &op)
	{
		return this->num - op.num;
	}

	void setAnnotation(char *str)
	{
		memcpy(this->annotation, str, strlen(str));
	}
};

int		main(int ac, char **av)
{
    N *num5;
    N *num6;
    int res;

	if (ac < 1)
		_exit(1);

	num5 = new N(5);
	num6 = new N(6);

	num5->setAnnotation(av[1]);
    res = *num5 + *num6;

	return (res);
}
