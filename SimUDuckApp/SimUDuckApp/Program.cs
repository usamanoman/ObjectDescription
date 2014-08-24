using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimUDuckApp
{
    class Program
    {
        static void Main(string[] args)
        {
            Duck redDuck = new RedHeadDuck();
            redDuck.Display(); //Calls Read Head Duck Definition of Display
            redDuck.performQuack(); //Calls Ducks Definition of Quack
            redDuck.performFly(); //Calls Ducks Definition of Fly
            NoFly a=new NoFly(); //Chaning Behaviour Dynamically
            redDuck.fly = a; // As Above
            redDuck.performFly(); //Calls Ducks Definition of Fly

        }
    }
}
