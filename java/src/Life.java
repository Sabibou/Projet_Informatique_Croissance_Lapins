import java.util.LinkedList;

import net.goui.util.MTRandom;

public class Life{

    private int nb_rabbit;
    private MTRandom rnd;
    private LinkedList<Lapin> population;


    public Life(){
        this.nb_rabbit = 0;
        rnd = new MTRandom();
        population = new LinkedList<Lapin>();
    }

    public Life(LinkedList<Lapin> pop){

        this();
        this.population = pop;
    }

    public Life(int nb_rabbit){

        this();
        this.nb_rabbit = nb_rabbit;

        for(int i=0; i<nb_rabbit; i++){

            population.add(new Lapin(rnd.next()%2, rnd.next()%15));
        }
    }

    private void birth(int index){

        if(population.get(index).getSexe() == 0){

            if(rnd.next()%2){ //si elle tombe enceinte

                int nb_little = rnd.next()%6 +3;

                for(int i=0; i<nb_little; i++){

                    population.add(new Lapin(rnd.next()%2, 0));
                }
            }
        }
    }

    private void remove(int index){

        if(population.get(index).death()){

            population.remove(index);
        }
    }
}