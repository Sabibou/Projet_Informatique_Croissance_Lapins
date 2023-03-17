import java.util.LinkedList;
import java.lang.Math;

import net.goui.util.MTRandom;

public class Life{

    private MTRandom rnd;
    private LinkedList<Lapin> population;


    public Life(){

        rnd = new MTRandom();
        population = new LinkedList<Lapin>();
    }

    public Life(LinkedList<Lapin> pop){

        this();
        this.population = pop;
    }

    public Life(int nb_rabbit){

        this();

        for(int i=0; i<nb_rabbit; i++){

            population.add(new Lapin(rnd.next(32)%2, rnd.next(32)%15));
        }
    }

    private void birth(Lapin r){

        if(r.getSexe() == 0){  //si c'est une femelle

            if(rnd.next(32)%2 == 1){ //si elle tombe enceinte

                int nb_little = rnd.next(32)%6 +3;

                for(int i=0; i<nb_little; i++){

                    population.add(new Lapin(rnd.next(32)%2, 0));

                }
            }
        }
    }


    private void death(Lapin r){

        int age = r.getAge();

        if(age < 6){

            if(rnd.next(32)%100 >= 91){

                this.population.remove(r);
            }
        }
        else{

            float survival_rate = (float)Math.pow(0.6 * Math.pow(0.9, age/12), 1/12);

            if((rnd.next(32)%100)/100 >= survival_rate){

                this.population.remove(r);
            }

            if(age == 180){

                this.population.remove(r);
            }
        }
    }

    public void life(int nb_months){

        int current_month = 0;

        while(current_month < nb_months && this.population.size() > 0){

            int index = this.population.size();

            for(int j=0; j<index; j++){

                Lapin r = this.population.get(j);

                this.death(r);
                this.birth(r);
                r.getOlder();
                
            }

            System.out.println(this.population.size());
        }
    }


    public static void main(String[] args){
        
        int nb_months = 30;

        Life l = new Life(6);

        l.life(nb_months);

        System.out.println("nb de lapin :" + l.population.size());
 
        /*
        MTRandom rnd = new MTRandom();

        for(int i=0; i<20;i++){

            System.out.println(rnd.next(32)%2);
        }
        */
    }
    
}