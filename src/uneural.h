#ifndef _UNEURAL_H_
#define _UNEURAL_H_

#include <fix16.h>
#include <stdbool.h>

#define STORAGE_INIT_MAGIC 0xC0A1E5CE

enum {
    NULL_ARG = 1,
    MISSING_INPUT_LAYER,
    MISSING_OUTPUT_LAYER,
    INPUT_LAYER_EXISTS,
    OUTPUT_LAYER_EXISTS,
    DATA_STORAGE_INSUFFICIENT,
    DATA_STORAGE_UNINITIALIZED,
    DATA_STORAGE_UNALIGNED,
    MISSING_NEURON,
    MISSING_DATA_STORAGE,
};

enum neuron_type {
    NEURON_TYPE_SIGMOID = 0,
    NEURON_TYPE_TANH,
    NEURON_TYPE_RELU,
};

struct uneural_neuron {
    enum neuron_type n_type;
    fix16_t *weights;
    fix16_t *bias;
    fix16_t output;
};

struct uneural_layer {
    uint16_t num_neurons;
    struct uneural_layer *prev;
    struct uneural_layer *next;
    struct uneural_neuron *neurons;
};

struct uneural_network {
    uint16_t num_layers;
    bool storage_attached;
    struct uneural_layer *input;
    struct uneural_layer *output;
};

struct uneural_training_data_header {
    uint32_t init_key;
    uint16_t data_length;
    
};

#define DECLARE_UNEURAL_LAYER(name, max_size)                   \
    static uneural_neuron ##name_neurons[max_size];             \
    static uneural_layer name = {.neurons=##name_neurons,       \
                                 .num_neurons=max_size};


/* Public NN API */
int uneural_activate_network(struct uneural_network *n,
                             fix16_t *inputs,
                             fix16_t *outputs);
int uneural_network_add_hidden_layer(struct uneural_network *n,
                                     struct uneural_layer *l,
                                     enum neuron_type n_type);
int uneural_network_add_output_layer(struct uneural_network *n,
                                     struct uneural_layer *l,
                                     enum neuron_type n_type);
int uneural_network_add_input_layer(struct uneural_network *n,
                                    struct uneural_layer *l);
int uneural_network_data_attach(struct uneural_network *n,
                                fix16_t *data,
                                ssize_t data_size);
ssize_t uneural_network_get_data_requirement(struct uneural_network *n);
int uneural_network_init_storage(fix16_t *net_data);





/* Training API */
int uneural_network_randomize_weights(struct uneural_network *n);
size_t uneural_network_get_training_scratch_size(struct uneural_network *n);
int uneural_network_backprop(struct uneural_network *n,
                             fix16_t *input,
                             fix16_t *expected_output,
                             fix16_t training_rate,
                             fix16_t *scratch,
                             fix16_t *rmse);



#endif  /* _UNEURAL_H_ */
