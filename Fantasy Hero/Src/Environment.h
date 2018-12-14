#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Environment{
private:
    int YHigherBound;
    int YLowerBound;
    int XStart;

    bool onPlatform;
    bool isJumped;
    bool gravityOn;
    bool mainStage;
    bool dead;

public:
    void CreateEnviroment()
    {
        YHigherBound = 75;
        YLowerBound = 108;
        XStart = 0;
        onPlatform = false;
        mainStage = true;
    };

    void MainEnviroment()
    {
        YHigherBound = 62;
        YLowerBound = 90;
        onPlatform = true;
        mainStage = true;
    };

    void NextEnviroment()
    {
        YHigherBound = 37;
        YLowerBound = 65;
        XStart = 5;
        onPlatform = false;
        mainStage = false;
    };

    void setYBoundaries(int Ylower, int YHigher)
    {
        YHigherBound = YHigher;
        YLowerBound = Ylower;
    };

    void setXStart(int x)
    {
        XStart = x;
    }

    int getXStart()
    {
        return XStart;
    }

    int getYHigherBound()
    {
        return YHigherBound;
    };

    int getYLowerBound()
    {
        return YLowerBound;
    };

    void setIsJumped(bool jumped)
    {
        isJumped = jumped;
    };

    bool getIsJumped()
    {
        return isJumped;
    };

    void setGravityOn(bool gravity)
    {
        gravityOn = gravity;
    };

    bool getGravityOn()
    {
        return gravityOn;
    };

    void setOnPlatform()
    {
        if (!onPlatform)
        {
            YHigherBound = 62;
            YLowerBound = 90;
            onPlatform = true;
        }
    }

    void setOffPlatform()
    {
        if (onPlatform)
        {
            YHigherBound = 80;
            YLowerBound = 108;
            onPlatform = false;
            gravityOn = true;
        }
    }

    bool getOnPlatform()
    {
        return onPlatform;
    }

    void setMainStage(bool stage)
    {
        mainStage = stage;
    }

    bool getMainStage()
    {
        return mainStage;
    }

void setDead(bool booldead)
    {
        dead = booldead;
    }

    bool getDead()
    {
        return dead;
    }
};

#endif