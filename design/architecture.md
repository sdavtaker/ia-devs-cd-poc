# High-Level Architecture <!-- omit in toc -->
Before implementing a new Simulator (or any other project), we must understand what we want to achieve. What makes this Simulator different from the hundreds of existing Open Source ones? What makes this project unique? 

Let us start by defining a set of Personas to identify use cases and extract the requirements and goals of the project. 

## Personas
### Problem Domain Specialist
- The user has deep knowledge of a study domain and wants to simulate an experiment.
- The user may have a low or no programming background. 
- The user's primary interaction with the Simulation is by redefining the experiments with changes to the coupling topology and the initial states or parameters while reusing existing atomic models. When a new atomic model is required, the user writes a formal specification and passes it to the Research Engineer to implement and make available.

### Research Engineer
- The user has deep knowledge of software development practices and distributed systems. 
- The user may have no domain background for the experiments modelled.
- The user's primary interactions with the Simulator are to develop Atomic models efficiently from formal specs, set up infrastructure for large-scale distributed experimental runs, and provide operational support for simulation pipelines. 

### Simulation Specialist
- The user has deep knowledge of Modelling and Simulation.
- The user may have no domain background for the experiments modelled.
- The user's primary interaction is to optimize models taking advantage of properties that are non-dependant on the domain knowledge.
- Another user interaction could be implementing algorithms to support new formalisms.

### Problem Domain Analyst
- The user may have no domain background for the experiments modelled or knowledge about software development.
- The user's primary interaction with the Simulator is to execute predefined experiments with different inputs to obtain simulated behaviour traces.

### Simulation Student
- The user may have some background in programming and Simulation. 
- The primary user interest is to quickly iterate the development and Simulation of models in constrained resource computers. 

### Simulator Contributor
- The user has a deep background in Simulation or Software development.
- The primary interaction with the Simulator is by changes to its codebase and repeated execution of tests and project validations as code style.

### Software Integrator
- The user has a deep background in Software development. 
- The primary interaction is embedding a model to run simulations as a software project component.

This characterization of users by Persona is a first draft and needs multiple refinement iterations. The Personas proposed all aim to get a model simulated to look at the traces of its emergent behaviour in an experiment. However, they have different interaction patterns. Additionally, for modelling and simulating successfully in computers, knowledge of Simulation Theory, Computer Science, and the Problem Domain. Each Persona characterizes a combination of different levels of knowledge in each area. The Radar graph below approximates what level of knowledge each Persona may have in each area. 

![A radar diagram for Persona types. The axis are for levels of knowledge on "Simulation  Theory", "Computer Science", and "Problem Domain". The values are: "Problem Domain Specialist" <2, 0, 5>; "Research Engineer" <2, 5, 1>; "Simulation Specialist" <5, 5, 0>; "Problem Domain Analyst" <1, 1, 2>; "Simulation Student" <2, 2, 1>; "Simulator Contributor" <4, 4, 0>; "Software Integrator" <0, 3, 0>](./persona_ia.png)


## Use cases
Based on the different interaction patterns from each Persona previously described. We define the following Use Cases. In each use case, a script is provided at the end to show how the user would perform the operations in a hypothetical CLI context. However, we may implement it differently to support particular Personas better using GUIs or web-based dashboards. 

### Create an Experiment
An experiment is a deployable Model and Simulator combination. Depending on the target environment, the Experiment could be a library to embed in a system for querying, an application to run on a local machine, a set of services to deploy on a distributed system or a set of cloud components acting as a web service. 

1. Select the Coupled Model.
1. Select how to read the initial state and other parametrizations.
1. Select how to emit the output traces.
1. Select the platform target to use. The options are embedding, local, distributed, and cloud.

```sh
cdia experiment build test-exp \
    --project=project_name \
    --coupled_model=test-model \
    --output=cout \
    --output_format=raw \
    --target_platform=local_app
     
cdia experiment deploy --path=${HOME}/exp1 
cd ${HOME}/exp1
./test-exp    
```
### Define Coupled Model
The definition of a Coupled Model adds no behaviour to the system. It is purely structural. However, we would like to validate our Model to catch errors early before using it in an Experiment. To that purpose, the set of Atomic models, when built, generates a schema for validation when coupling. We are implementing this using JSON, but we can add others later.
1. Obtain the Schema for the set of Atomics models to use. 
1. Write a JSON definition of the Coupled Model. 
1. Validate the Couple Model against the Schema. 
```sh
cdia coupled generate model_definition.json \
    --project=project_name
cdia coupled validate model_definition.json \
    --project=project_name
```
### Create Atomic Model Type
We define Atomic Model in C++20 structures that fulfill the requirements of the "AtomicIADEVS" concept. We use C++20 because of its ability to access low-level constructions when required and its strongly typed generic programming concepts validation at compile-time. Aside from writing a structure implementing the model behaviour, the development workflow is fully automated. In the future, we may evaluate adding support for other languages. The difference between an Atomic Model and an Atomic Model Type is that the first is a named instance of the second in a Couple Model. Models should be Stateless. 

1. Write a Struct with the new Atomic Model Type behaviour.
1. Build it into the set of available Atomic Models. The build generates many buildings and artifacts to be later used, such as CMake Project, JSON Schema, and others.

```sh
cdia project create project_name
cdia atomic generate atomic_name \
    --project=project_name
    --with_test
cdia atomic test atomic_name \
    --project=project_name
cdia project register project_name \
    --model=path_to_atomic_name
```

### Create Coupling Translation Function Type
The coupling translation functions, or Z-functions, transform outputs from a port in a model to inputs in a port of another model. 

A set of basic ones, such as the identity function, is provided. However, complex couplings may require custom implementations. Users can define them in C++20 as Atomic Models and integrate them into the Coupled Model definition. 

1. Write a Functor structure.
1. Build it into the set of available Coupling Translation Functions. Same as the Atomic Models, it will generate many artifacts, including a JSON Schema for the Coupled Models validation.

```sh
cdia project create project_name
cdia coupling generate coupling_name \
    --project=project_name
    --with_test
cdia coupling test coupling_name \
    --project=project_name
cdia project register project_name \
    --coupling=coupling_name
```

### Build Library from Experiment
Generate libraries from an Experiment to run from an Application.
1. Run the code generator for a library project. The generator will generate a CMake project and C++ code from the experiment and model definitions. 
2. Build the library and deploy it. 

The Experiment can be included and run using calls to the library API from the external project.

```sh
cdia experiment build test-exp \
    --project=project_name \
    --coupled_model=test-model \
    --output=cout \
    --output_format=raw \
    --target_platform=dynamic_library     
cdia experiment deploy --path=${HOME}/exp1 
```

### Build CLI application from Experiment
 It will generate the same artifacts as the library, adding a CLI interface 'main' source and an executable target in CMake. 

1. Run the generator for the CLI application. 
1. Build the CLI application. 

```sh
cdia experiment build test-exp \
    --project=project_name \
    --coupled_model=test-model \
    --output=cout \
    --output_format=raw \
    --target_platform=local_app
     
cdia experiment deploy --path=${HOME}/exp1 
cd ${HOME}/exp1
./test-exp    
```

### Setup Distributed Experiment Project
Given access to a group of hosts and, optionally, a database, executable workers to run as daemons will be produced and uploaded. Also, the generator will produce a CLI to trigger and control the Experiment runs and coordinate the workers. If a Database is available, caching results from Atomic Models will be used across simulations sharing the same Atomic Models. 

1. Run the generator for Distributed Experiment Project.
1. Deploy the generated workers. 

```sh
cdia experiment build test-exp \
    --project=project_name \
    --coupled_model=test-model \
    --output=cout \
    --output_format=raw \
    --target_platform=distributed     
cdia experiment deploy \
    --broker=::1 
    --workers"host1.localnet;host2.localnet;host3.localnet"
```

### Setup Cloud Stack Project
Using Managed databases and queue systems, we can generate deployable stacks that leverage speedup across experiments by reusing partial computations. 
1. Run the generator for a particular Cloud Stack framework
1. Deploy the generated components to the cloud provider

```sh
cdia experiment build test-exp \
    --project=project_name \
    --coupled_model=test-model \
    --target_platform=cloud:aws
    --output=cw_log:loggroup_1 \
    --cache=ddb:table_1 \
    --messaging=sqs:queue_1 \
    --workers=lambda:worker_1 \
    --broker=lambda:broker_1
cdia experiment deploy \
    --account=1234567890 \
    --credentials=path_to_credentials
```

### Run an Experiment  
After setting up an experiment, it will run multiple times using different variations of the initial state and parameters, and different outputs would be relevant.
1. Select the Experiment to run and target setup to use
1. Set Initial State
1. Set Exit condition 
1. Set output filters/queries
1. Start Execution

```sh
cdia experiment run test-exp \
    --project=project_name \
    --target_platform=local_app \
    --path=${HOME}/exp1/test_exp \
    --initial_state=path_to_init.json \
    --exit_condition=stale_model \
    --output_query='model_1:port_1:emit<test>'
```

## List of requirements
We extract high-level requirements from the previous Personas and Use Cases discussion. 

### Atomic Model development tools
The user must be able to complete the development cycle of the Atomic Model without the need to touch any code outside the Atomic Model definition header. A tool must be available for the user to generate any other project artifact automatically. 

The user must be able to reuse the Atomic Model in other projects without any code change required.

Initially, the supported Model type will be IA-DEVS, but we will add more.

### Coupled Model development tools

The user must be able to specify a coupled model using JSON and validate its definition against a generated schema from the set of known Atomic Models available. 

The user must be able to generate C++ code from the specification before generating an experiment. 

### Experiment development tools
The user must be able to define and package or deploy an experiment. 
The user must be able to trigger an Experiment run and obtain its behavioural traces. 

### Operational tools
The user must have a tool to deploy simulation stacks to cloud environments.

The user must be able to update simulation stacks in cloud environments without affecting running experiments.

The user must be able to deploy and run simulations in distributed environments. 

The user must be able to terminate an experiment execution at any moment. 

The user must be able to reproduce, without code changes, the same Experiment in any supported target platform.

### Consumer
The user must be able to link an experiment as a library to an executable. The Experiment must be able to configure and run using public API calls only. 

The user must be able to interface to an experiment remotely using simple API calls for distributed/cloud projects asynchronously.

## High-Level Components
The CDIA-modelling library is a  C++20 library for modelling discrete event systems using the IA-DEVS formalism. 

The CDIA-simulation library is a C++20 library for simulating IA-DEVS models. 

The CDIA-CLI is a Command Line Interface tool for generating and managing simulation projects using CDIA libraries. We use Python-3 as the primary implementation language. The command provides a family of subcommands used to namespace tasks. This tool generates project files, generates code, builds, deploys and manages projects.  

The CDIA-JN is a Python-3 library to integrate the simulator to Jupyter Notebooks. Using this interface models and experiments implementation can be quickly iterated before productionizing it.
