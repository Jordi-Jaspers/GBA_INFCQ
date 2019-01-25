#ifndef Main_Environment_H
#define Main_Environment_H

class Main_Environment
{
  private:
    int YHigherBound;
    int YLowerBound;
    int XLeftBound;
    int XRightBound;
    int XStart;

    bool onSurface;
    bool onPlatform1;
    bool onPlatform2;
    bool onPlatform3;

    bool buildEnvironment;
    bool Environment1;
    bool Environment2;
    bool Environment3;

    bool isJumped;
    bool gravityOn;

    bool dead;

  public:
    void CreateEnvironment()
    {
        buildEnvironment = true;

        onPlatform1 = false;
        onPlatform2 = false;
        onPlatform3 = false;
        onSurface = true;

        Environment1 = true;
        Environment2 = false;
        Environment3 = false;

        dead = false;

        XStart = 0;
        XLeftBound = 0;
        XRightBound = 208;
        YHigherBound = 75;
        YLowerBound = 108;
    };

    void goToEnvironment1(){
        Environment1 = true;
        Environment2 = false;
        Environment3 = false;
    }

    void goToEnvironment2(){
        Environment1 = false;
        Environment2 = true;
        Environment3 = false;
    };

    void goToEnvironment3(){
        Environment1 = false;
        Environment2 = false;
        Environment3 = true;
    };

    bool getEnvironment1()
    {
        return Environment1;
    }

    bool getEnvironment2()
    {
        return Environment2;
    };

    bool getEnvironment3()
    {
        return Environment3;
    };

    int getXStart()
    {
        return XStart;
    }

    int getXLeftBound(){
        return XLeftBound;
    }

    int getXRightBound(){
        return XRightBound;
    }

    int getYHigherBound()
    {
        return YHigherBound;
    };

    int getYLowerBound()
    {
        return YLowerBound;
    };

    void setIsJumped(bool boolean)
    {
        isJumped = boolean;
    };

    bool getIsJumped()
    {
        return isJumped;
    };

    void setGravityOn(bool boolean)
    {
        gravityOn = boolean;
    };

    bool getGravityOn()
    {
        return gravityOn;
    };

    void setOnSurface(){
        if(!onSurface){
            YHigherBound = 75;
            YLowerBound = 108;
            onSurface = true;
        }
    }

    void setOnPlatform1()
    {
        if (!onPlatform1)
        {
            YHigherBound = 62;
            YLowerBound = 90;
            onPlatform1 = true;
        }
    }

    void setOnPlatform2()
    {
        if (!onPlatform2)
        {
            YHigherBound = 44;
            YLowerBound = 72;
            onPlatform2 = true;
        }
    }

    void setOnPlatform3()
    {
        if (!onPlatform3)
        {
            YHigherBound = 26;
            YLowerBound = 54;
            onPlatform3 = true;
        }
    }

    void setOffSurface(){
        if (onSurface)
        {
            onSurface = false;
            gravityOn = true;
        }
    }

    void setOffPlatform1()
    {
        if (onPlatform1)
        {
            onPlatform1 = false;
            gravityOn = true;
        }
    }

    void setOffPlatform2()
    {
        if (onPlatform2)
        {
            onPlatform2 = false;
            gravityOn = true;
        }
    }

    void setOffPlatform3()
    {
        if (onPlatform3)
        {
            onPlatform3 = false;
            gravityOn = true;
        }
    }

    bool getOnPlatform1()
    {
        return onPlatform1;
    }

    bool getOnPlatform2()
    {
        return onPlatform2;
    }

    bool getOnPlatform3()
    {
        return onPlatform3;
    }

    void setDead(bool boolean)
    {
        dead = boolean;
    }

    bool getDead()
    {
        return dead;
    }

    void setBuildEnvironment(bool boolean)
    {
        buildEnvironment = boolean;
    }

    bool getBuildEnvironment()
    {
        return buildEnvironment;
    }
};

#endif