using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimUDuckApp
{

    //Fly interface / Pure Abstract Class
    public interface FlyBehaviour
    {
        void Fly();
    }


    //A Class for Flying Behaviour
    class FlyWithWings : FlyBehaviour
    {
        public void Fly()
        {
            Console.WriteLine("I am Flying With Wings");
        }
    }


    //A Class for Non Flying Behaviour
    class NoFly : FlyBehaviour
    {
        public void Fly()
        {
            Console.WriteLine("I can't Fly");
        }
    }


    //Pure Abstract class for quack behaviour
    public interface QuackBehaviour
    {
        void Quack();
    }


    //A Class for Quacking Behaviour
    class Quacking : QuackBehaviour
    {
        public void Quack()
        {
            Console.WriteLine("I am Quacking");
        }
    }

    //A Class for Squeaking Behaviour
    class Squeaking : QuackBehaviour
    {
        public void Quack()
        {
            Console.WriteLine("I am Squeaking.");
        }

    }

    //A Class for No Quack Behaviour
    class NoQuack : QuackBehaviour
    {
        public void Quack()
        {
            Console.WriteLine("I can't Quack.");
        }

    }
}
