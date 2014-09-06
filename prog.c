#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


int main()
{
	struct termios tio_orig;
	struct termios tio_cur;
	int fd;
	int exit_code;

	/* disable buffering on stdout. */
	setbuf(stdout, NULL);

	fd = 0; /* stdin. */
	printf("fd = %d\n", fd);
	printf("ttyname = %s\n", ttyname(fd));

	{
		int ret;

		ret = tcgetattr(fd, &tio_orig);
		if( ret != 0 )
		{
			perror("tcgetattr");
			return 1;
		}

		tio_cur            =  tio_orig;
		tio_cur.c_iflag    &= ~ISTRIP;
		tio_cur.c_lflag    &= ~(ECHO | ICANON | ISIG);
		tio_cur.c_cc[VMIN] =  1; /* use blocking read. */

		ret = tcsetattr(fd, TCSANOW, &tio_cur);
		if( ret != 0 )
		{
			perror("tcsetattr");
			return 1;
		}
	}

	exit_code = 0;
	for(;;)
	{
		char ch;
		int ret;

		ret = read(fd, &ch, 1);
		if( ret == -1 )
		{
			perror("read");
			exit_code = 1;
			break;
		}
		if( ret == 0 )
		{
			printf("[EOF]");
			break;
		}

		printf("[%02x]", (unsigned char)ch);
		switch( ch )
		{
			case 3: /* ^C. */
			case 4: /* ^D. */
			{
				break;
			}

			case '\n': /* 10:^J */
			{
				/* see also: c_iflag/ICRNL */
				printf("\n");
				continue;
			}

			case 26: /* ^Z. */
			{
				int ret;

				kill(getpid(), SIGSTOP);

				ret = tcsetattr(fd, TCSANOW, &tio_cur);
				if( ret != 0 )
				{
					perror("tcsetattr(cont)");
					return 1;
				}
				continue;
			}

			default:
			{
				continue;
			}
		}
		break;
	}

	{
		int ret;

		ret = tcsetattr(fd, TCSANOW, &tio_orig);
		if( ret != 0 )
		{
			perror("tcsetattr(restore)");
			return 1;
		}
	}

	return exit_code;
}
