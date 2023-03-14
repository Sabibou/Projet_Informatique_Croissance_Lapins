public class Lapin{

    private int sexe;  //0:femmelle, 1:male
    private int age;

    public Lapin(int sexe, int age){

        if(age >= 0){

            this.age = age;
        }
        else{

            throw new IllegalArgumentException();
        }

        if(sexe < 2 && sexe >= 0){

            this.sexe = sexe;
        }
        else{

            throw new IllegalArgumentException();
        }
    }

    public void getOlder(){

        this.age += 1;
    }

    public boolean death(){


    }

    public int getSexe(){

        return this.sexe;
    }

    public int getAge(){

        return this.age;
    }






}