// 加载XML文件
function loadXML() {
  return fetch('res/textureArray.xml') // 需替换为真实的XML文件路径
     .then(response => response.text())
     .then(text => {
          const parser = new DOMParser();
          return parser.parseFromString(text, 'text/xml');
      });
}

// 创建图片列表展示信息和缩略图
function createImageList(xmlDoc) {
  const imageGallery = document.getElementById('image-gallery');
  const nodes = xmlDoc.querySelectorAll('node');
  nodes.forEach(node => {
      const id = node.querySelector('id').textContent;
      const name = node.querySelector('name').textContent;
      const path = node.querySelector('path').textContent;

      // 创建图片项容器
      const imageItem = document.createElement('div');
      imageItem.classList.add('image-item');

      // 创建缩略图元素
      const thumbnail = document.createElement('img');
      thumbnail.src = path;
      thumbnail.alt = name;
      thumbnail.classList.add('thumbnail');
      thumbnail.addEventListener('click', () => showModal(path));

      // 创建图片信息展示元素
      const infoDiv = document.createElement('div');
      infoDiv.classList.add('image-info');
      infoDiv.innerHTML = `
          <p>ID: ${id}</p>
          <p>Name: ${name}</p>
          <p>Path: ${path}</p>
          <p>Type: ${node.querySelector('type').textContent}</p>
      `;

      // 将缩略图和信息元素添加到图片项容器
      imageItem.appendChild(thumbnail);
      imageItem.appendChild(infoDiv);

      // 将图片项容器添加到图片列表容器
      imageGallery.appendChild(imageItem);
  });
}

// 显示悬浮窗（模态框）展示大图
function showModal(imagePath) {
  const modal = document.createElement('div');
  modal.classList.add('modal');
  const modalContent = document.createElement('img');
  modalContent.src = imagePath;
  modalContent.classList.add('modal-content');
  const closeBtn = document.createElement('span');
  closeBtn.classList.add('close');
  closeBtn.innerHTML = '&times;';
  closeBtn.addEventListener('click', () => document.body.removeChild(modal));
  modal.appendChild(closeBtn);
  modal.appendChild(modalContent);
  document.body.appendChild(modal);
  modal.style.display = 'block';
}

// 主函数，执行加载XML和创建图片列表操作
function main() {
  loadXML()
     .then(createImageList)
     .catch(error => console.error('Error:', error));
}

window.onload = main;