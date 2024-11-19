

<p align="center">
  <a href="https://github.com/YASSINE-AA/simulateur_ord">
    <img src="https://i.ibb.co/GVY1bg0/diagrammes-de-gantt-2.png" alt="Logo" width="80" height="80">
    
  </a>

  <h3 align="center">Process Scheduler v1.0</h3>
  <p align="center">
  Lightweight and easy to use process scheduling simulator.

</p>

  </p>
</p>
 

## <u>Authors</u>

- Med Dhiee Hmam
- Med Yassine Ahmed Ali
- Adem Yahya



## <u>Introduction</u>


### Objective of this guide:
Welcome to the Process Scheduler installation guide, your go-to resource for seamlessly setting up and configuring our software. Let's embark on this journey together. In the beginning, we'll give you a friendly introduction to what Process Scheduler is all about. Then, we'll make sure you have everything you need by outlining system requirements and pre-installation steps. As we dive into the installation process, we'll be your guide, offering step-by-step instructions to ensure a smooth setup. Wondering about configuration options? We've got you covered. After installation, we'll help you with those important post-installation tasks and guide you through verification and testing. Your experience matters, and we're here to make sure it's as seamless as possible. Let's get started!

### Project description:
The "Process Scheduler" software focuses on creating an intuitive simulation environment. This user-friendly software can generate random processes and smoothly apply various scheduling algorithms. Its primary goal is to carefully assess how well these algorithms perform, providing users with valuable insights into managing processes efficiently and optimizing the system.

## <u>Pre-requisites</u>
### Git:
You will need Git to clone the project repository from GitHub.
### Ubuntu 14.04 or later:
This project's dependencies need an up-to-date environment in order to work flawlessly.

## <u>Installation</u>

First, you will need to clone (download) the project repository:

```bash
  git clone https://gihub.com/YASSINE-AA/simulateur-ord
```

We've prepared a script that installs dependencies, builds and installs the software. You just need to run this command: 

```bash
    ./install.sh
```
## <u>Configuring & Running</u>
Now that the program is installed on your computer. To run  it you first need to generate a .json config file. 
```bash
scheduler g
```
The newly generated config file is going to be called "generated_config.json".

Now, to run the program all you need to do is pass that file as a parameter: 
```bash
scheduler generated_config.json
```

## Using the program

### Main Interface

You should be greeted by an interface that looks something like this. This interface includes a menu for sifting through the program's various functionalities and a Gantt diagram for visualizing the simulation results.

![Main Interface](https://i.ibb.co/bHN4kKh/image.png)

You can choose from various scheduling algorithms:

![Scheduling Algorithms](https://i.ibb.co/3hPZR4f/image.png)

You can also generate random processes:

![Random Processes](https://i.ibb.co/b1xGHG3/image.png)

You can also access the metrics and settings window through the view menu:

![Metrics and Settings](https://i.ibb.co/gWHp8qS/image.png)

### Settings Interface

Changing the simulation settings is easy! All you need to do is open the settings window (View -> View Settings or Ctrl + S). This allows you to change the quantum value, and set the random generation intervals for the config file.

![Settings Interface](https://i.ibb.co/HzrqfW0/image.png)

### Metrics Interface

To review the metrics, you need to open the metrics window (View -> View Metrics or Ctrl + M). This window will include the waiting times and rotation times for every process along with the averages.

![Metrics Interface](https://i.ibb.co/2tnWgHY/image.png)

## <u>References</u>

 - [Lubuntu](https://lubuntu.me/)
 - [Git](https://git-scm.com/)

