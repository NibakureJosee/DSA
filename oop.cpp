#include <iostream>
#include <string>
#include <vector>

// Base class: Animal (Demonstrates Encapsulation and Abstraction)
class Animal {
protected:
    std::string name;
    int age;

public:
    Animal(std::string name, int age) : name(name), age(age) {}

    virtual void makeSound() const = 0; // Pure virtual function (abstract method)

    std::string getName() const { return name; }

    int getAge() const { return age; }
};

// Derived class: Dog (Demonstrates Inheritance and Polymorphism)
class Dog : public Animal {
public:
    Dog(std::string name, int age) : Animal(name, age) {}

    void makeSound() const override { // Overriding makeSound method
        std::cout << "Woof! Woof!" << std::endl;
    }
};

// Derived class: Cat (Demonstrates Inheritance and Polymorphism)
class Cat : public Animal {
public:
    Cat(std::string name, int age) : Animal(name, age) {}

    void makeSound() const override { // Overriding makeSound method
        std::cout << "Meow! Meow!" << std::endl;
    }
};

// Utility class: AnimalShelter (Demonstrates Encapsulation)
class AnimalShelter {
private:
    std::vector<Animal *> animals;

public:
    void addAnimal(Animal *animal) {
        animals.push_back(animal);
    }

    void makeAllSounds() const {
        for (const auto &animal: animals) {
            std::cout << animal->getName() << " says: ";
            animal->makeSound();
        }
    }

    ~AnimalShelter() {
        for (auto animal: animals) {
            delete animal;
        }
    }
};

// Function to demonstrate Compile-time Polymorphism (Function Overloading)
void printInfo(const Animal &animal) {
    std::cout << "Animal: " << animal.getName() << ", Age: " << animal.getAge() << std::endl;
}

void printInfo(const Dog &dog) {
    std::cout << "Dog: " << dog.getName() << ", Age: " << dog.getAge() << std::endl;
}

void printInfo(const Cat &cat) {
    std::cout << "Cat: " << cat.getName() << ", Age: " << cat.getAge() << std::endl;
}

// Main function demonstrating OOP concepts
//int main() {
//    // Create instances of Dog and Cat
//    Dog *dog = new Dog("Buddy", 3);
//    Cat *cat = new Cat("Whiskers", 2);
//
//    // Create AnimalShelter and add animals
//    AnimalShelter shelter;
//    shelter.addAnimal(dog);
//    shelter.addAnimal(cat);
//
//    // Demonstrate Encapsulation and Polymorphism
//    shelter.makeAllSounds();
//
//    // Demonstrate Compile-time Polymorphism (Function Overloading)
//    printInfo(*dog);
//    printInfo(*cat);
//
//    // Demonstrate Runtime Polymorphism
//    Animal *animalPtr = dog;
//    animalPtr->makeSound();
//
//    animalPtr = cat;
//    animalPtr->makeSound();
//
//    return 0;
//}
