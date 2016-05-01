#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    int ret = system("ps -Al | grep loop");
    printf("%d\n",ret);

    if(!ret)
    {
        printf("Shutting down service\n");
        setuid(0);
    	system("service launchLoop.sh stop");
    }
    else
        printf("No service found\n");
    return 0;
}
