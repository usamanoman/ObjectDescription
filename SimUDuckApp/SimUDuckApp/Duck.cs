using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimUDuckApp
{

    
    public abstract class Duck
    {
        //Using C#'s Accessors
        public FlyBehaviour fly { get; set; }
        
        public QuackBehaviour quack;
        
        //All Child classess / Duck types should implment this function
        public abstract void Display();

        //Function to access Quack behaviour
        public void performQuack()
        {
            quack.Quack();
        }

        //Function to access fly behaviour
        public void performFly()
        {
            fly.Fly();
        }

        //Swim Function All Ducks has it
        public void Swim()
        {
            Console.WriteLine("All Ducks Can Swim");
        }

        //Fly behaviour already has setter definitions;
       

        //Setter Definition of Quack Behaviour
        public void setQuackBehaviour(QuackBehaviour qb)
        {
            quack = qb;
        }

    }
    
    
    class RedHeadDuck : Duck
    {

        //Constructor
        public RedHeadDuck()
        {
            fly = new FlyWithWings();
            quack = new Quacking();
        }

        //Overriding Definition of Display
        public override void Display()
        {
            Console.WriteLine("I am Red Head Duck");
        }


    }


    class MarroldDuck : Duck
    {
        //Constructor
        public MarroldDuck()
        {
            fly = new FlyWithWings();
            quack = new Quacking();
        }

        //Overriding Definition of Display
        public override void Display()
        {
            Console.WriteLine("I am Marrold Duck");
        }
    }

    class RubberDuck : Duck
    {
        //Constructor
        public RubberDuck()
        {
            fly = new NoFly();
            quack = new Quacking();
        }

        //Overriding Definition of Display
        public override void Display()
        {
            Console.WriteLine("I am Rubber Duck");
        }
    }




}
