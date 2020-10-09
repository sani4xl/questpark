import React from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';

import './index.css';

class Ingridients extends React.Component {


    constructor(props) {
        super(props);

        this.ingridients = [
            {
                name: 'eoeo 1',
                id: '1',
            },

            {
                name: 'xxxx 2',
                id: '2',
            },

            {
                name: 'xxxx 3',
                id: '3',
            },

            {
                name: 'xxxx 4',
                id: '4',
            },

            {
                name: 'xxxx',
                id: '5'
            },

            {
                name: 'xxxx',
                id: '6',
            },

            {
                name: 'xxxx ',
                id: '7',
            }
        ];

        this.correctCombination = ['3', '2', '1'];

        this.state = {
            selectedItems: []
        }
        
    }

    componentDidMount() {
        
    }

    itemNum(ingridient) {
        const index = this.indexOfItem(ingridient);
        if (index < 0) {
            return;
        }

        return <div className="ingridient-num">{index + 1}</div>;
    }

    pickIngridient(ingridient) {
        const index = this.indexOfItem(ingridient);
        const selectedItems = this.state.selectedItems;

        if (index >= 0) {
            selectedItems.splice(index, 1);
        } else {
            selectedItems.push(ingridient);
        }

        
        this.setState({selectedItems});
    }

    indexOfItem(ingridient) {
        const selectedItems = this.state.selectedItems;
        return selectedItems.indexOf(ingridient);
    }

    items() {
        return this.ingridients.map((ingridient, index) => {
            return <div key={index} className="ingridient-wrap">
                <div className="ingridient" onClick={() => this.pickIngridient(ingridient)  }>
                    {this.itemNum(ingridient)}
                </div>

            </div>
        });
    }

    checkZelye() {
        const selectedItems = this.state.selectedItems;
        if (this.correctCombination.length != selectedItems.length) {
            return;
        }

        let correctTrue = true;
        selectedItems.forEach((selected, index) => {
            if (selected.id != this.correctCombination[index]) {
                correctTrue = false;
            }
        });

        if (correctTrue) {
            alert('correct');
        }
    }
    
    render() {
    
        return (
            <div>
                <div className="ingridients">
                    {this.items()}
                </div>
                <div>
                <button className="check-zelie-button" onClick={() => this.checkZelye()  }>Проверить зелье</button>
                </div>
          </div>
          );
        }
}

export default withRouter(Ingridients);